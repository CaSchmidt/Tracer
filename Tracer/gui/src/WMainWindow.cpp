/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <functional>

#include <QtConcurrent/QtConcurrentMap>
#include <QtCore/QThread>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "WMainWindow.h"
#include "ui_WMainWindow.h"

#include "rt/Camera/FrustumCamera.h"
#include "rt/Camera/SimpleCamera.h"
#include "rt/Loader/SceneLoader.h"
#include "rt/Renderer/DirectLightingRenderer.h"
#include "rt/Renderer/PathTracingRenderer.h"
#include "rt/Renderer/WhittedRenderer.h"
#include "rt/Sampler/SimpleSampler.h"
#include "rt/Scene/Scene.h"
#include "Util.h"

////// Macros ////////////////////////////////////////////////////////////////

#define HAVE_MANUAL_PROGRESS

#define CAM_FRUSTUM  QStringLiteral("Frustum")
#define CAM_SIMPLE   QStringLiteral("Simple")

#define METH_DIRECT   QStringLiteral("DirectLighting")
#define METH_PATH     QStringLiteral("PathTracing")
#define METH_WHITTED  QStringLiteral("Whitted")

////// public ////////////////////////////////////////////////////////////////

WMainWindow::WMainWindow(QWidget *parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
  , ui(new Ui::WMainWindow)
{
  ui->setupUi(this);

  // Initialize UI ///////////////////////////////////////////////////////////

  initializeImage();
  initializeProgress();
  initializeRender();
  initializeScene();
  initializeWork();

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->quitAction, &QAction::triggered, this, &WMainWindow::close);
}

WMainWindow::~WMainWindow()
{
  delete ui;
}

////// private ///////////////////////////////////////////////////////////////

void WMainWindow::initializeImage()
{
  ui->saveAsAction->setShortcut(QKeySequence::SaveAs);
  connect(ui->saveAsAction, &QAction::triggered, this, &WMainWindow::saveAs);

  ui->copyAction->setShortcut(QKeySequence::Copy);
  connect(ui->copyAction, &QAction::triggered, ui->imageWidget, &WImage::copyToClipboard);

  ui->imageWidget->setBackgroundBrush(createCheckedBrush(16));
}

void WMainWindow::initializeProgress()
{
  watcher.disconnect();

  ui->progressBar->setRange(0, 100);
  ui->progressBar->setValue(0);

  ui->startButton->setText(tr("Start"));
}

void WMainWindow::initializeRender()
{
  ui->methodCombo->clear();
  ui->methodCombo->addItems({METH_DIRECT, METH_PATH, METH_WHITTED});

  ui->cameraCombo->clear();
  ui->cameraCombo->addItems({CAM_FRUSTUM, CAM_SIMPLE});

  ui->samplesPerPixelCombo->setRange(1, 32768);
  ui->samplesPerPixelCombo->setValue(16);

  ui->widthSpin->setRange(128, 4096);
  ui->widthSpin->setValue(1000);

  ui->heightSpin->setRange(128, 4096);
  ui->heightSpin->setValue(1000);

  ui->gammaSpin->setRange(1, 10);
  ui->gammaSpin->setSingleStep(0.1);
  ui->gammaSpin->setValue(1);

  ui->maxDepthSpin->setRange(2, 32);
  ui->maxDepthSpin->setValue(5);
}

bool WMainWindow::initializeRenderContext()
{
  rc.clear();

  if( ui->sceneEdit->text().isEmpty() ) {
    return false;
  }
  const QString filename = ui->sceneEdit->text();

  // (1) Scene & RenderOptions ///////////////////////////////////////////////

  rc.scene = rt::Scene::create();
  rt::Scene *scene = rt::SCENE(rc.scene);

  rt::RenderOptions options;
  if( !rt::loadScene(scene, &options, filename.toUtf8().constData()) ) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Unable to load scene!"),
                          QMessageBox::Ok, QMessageBox::NoButton);
    return false;
  }
  scene->setUseCastShadow(ui->castShadowCheck->isChecked());

  options.gamma    = ui->gammaSpin->value();
  options.maxDepth = ui->maxDepthSpin->value();

  // (2) Renderer ////////////////////////////////////////////////////////////

  if(        ui->methodCombo->currentText() == METH_DIRECT ) {
    rc.renderer = rt::DirectLightingRenderer::create(options);
    rt::DIRECT_LIGHTING(rc.renderer)->setSampleOneLight(ui->sampleOneLightCheck->isChecked());
  } else if( ui->methodCombo->currentText() == METH_PATH ) {
    rc.renderer = rt::PathTracingRenderer::create(options);
  } else if( ui->methodCombo->currentText() == METH_WHITTED ) {
    rc.renderer = rt::WhittedRenderer::create(options);
  } else {
    QMessageBox::critical(this, tr("Error"),
                          tr("Invalid method!"),
                          QMessageBox::Ok, QMessageBox::NoButton);
    return false;
  }

  // (3) Camera //////////////////////////////////////////////////////////////

  const rt::size_t  width = ui->widthSpin->value();
  const rt::size_t height = ui->heightSpin->value();
  if(        ui->cameraCombo->currentText() == CAM_FRUSTUM ) {
    rc.camera = rt::FrustumCamera::create(width, height, rc.renderer->options());
  } else if( ui->cameraCombo->currentText() == CAM_SIMPLE ) {
    rc.camera = rt::SimpleCamera::create(width, height, rc.renderer->options());
  } else {
    QMessageBox::critical(this, tr("Error"),
                          tr("Invalid camera!"),
                          QMessageBox::Ok, QMessageBox::NoButton);
    return false;
  }

  // (4) Sampler /////////////////////////////////////////////////////////////

  rc.sampler = rt::SimpleSampler::create(ui->samplesPerPixelCombo->value());

  // (5) Blocks //////////////////////////////////////////////////////////////

  blocks = rt::makeRenderBlocks(height, ui->blockSizeSpin->value());

  // (6) Rendered Image //////////////////////////////////////////////////////

  QImage image(int(width), int(height), QImage::Format_RGBA8888);
  image.fill(Qt::black);
  ui->imageWidget->setImage(image);

  // (7) Final Check /////////////////////////////////////////////////////////

  if( !rc.isValid() ) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Invalid render context!"),
                          QMessageBox::Ok, QMessageBox::NoButton);
    return rc.isValid();
  }

  return rc.isValid();
}

void WMainWindow::initializeScene()
{
  connect(ui->openButton, &QPushButton::clicked, this, &WMainWindow::openScene);

  ui->castShadowCheck->setChecked(false);
}

void WMainWindow::initializeWork()
{
  connect(ui->startButton, &QPushButton::clicked, this, &WMainWindow::startWork);

  ui->numThreadsSpin->setRange(1, QThread::idealThreadCount());
  ui->numThreadsSpin->setValue(QThread::idealThreadCount());

  ui->blockSizeSpin->setRange(1, 128);
  ui->blockSizeSpin->setValue(8);
}

void WMainWindow::openScene()
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Open"),
                                                        QDir::currentPath(),
                                                        tr("Tracer Scenes (*.xml)"));
  if( filename.isEmpty() ) {
    return;
  }
  ui->sceneEdit->setText(filename);
  QDir::setCurrent(QFileInfo(filename).absolutePath());
}

void WMainWindow::saveAs()
{
  const QString filename = QFileDialog::getSaveFileName(this, tr("Save as"),
                                                        QDir::currentPath(),
                                                        tr("Images (*.jpg *.png)"));
  if( filename.isEmpty() ) {
    return;
  }
  ui->imageWidget->saveAs(filename);
  QDir::setCurrent(QFileInfo(filename).absolutePath());
}

void WMainWindow::startWork()
{
  if( watcher.isRunning() ) {
    watcher.cancel();
    watcher.waitForFinished();
    initializeProgress();

  } else {
    if( !initializeRenderContext() ) {
      return;
    }

    QThreadPool::globalInstance()->setMaxThreadCount(ui->numThreadsSpin->value());

#ifdef HAVE_MANUAL_PROGRESS
    ui->progressBar->setRange(0, int(blocks.size()));
    ui->progressBar->setValue(0);
#endif

    using Watcher = QFutureWatcher<Image>;
    connect(&watcher, &Watcher::finished,
            this, &WMainWindow::initializeProgress);
#ifndef HAVE_MANUAL_PROGRESS
    connect(&watcher, &Watcher::progressRangeChanged,
            ui->progressBar, &QProgressBar::setRange);
    connect(&watcher, &Watcher::progressValueChanged,
            ui->progressBar, &QProgressBar::setValue);
#endif
    connect(&watcher, &Watcher::resultReadyAt,
            this, &WMainWindow::updateResult);

    // NOTE: No Lambdas with captures for QtConcurrent::mapped() !!!
    using RenderFunc = std::function<Image(const rt::RenderBlock&)>;
    RenderFunc render = [&](const rt::RenderBlock& block) -> Image {
      return rc.render(block);
    };

    future = QtConcurrent::mapped(blocks, render);
    watcher.setFuture(future);

    ui->startButton->setText(tr("Cancel"));
  }
}

void WMainWindow::updateResult(int index)
{
  const rt::RenderBlocks::const_iterator it = std::next(blocks.begin(), index);
  const rt::size_t y0 = std::get<0>(*it);
  ui->imageWidget->insert(y0, watcher.resultAt(index));
#ifdef HAVE_MANUAL_PROGRESS
  ui->progressBar->setValue(ui->progressBar->value() + 1);
#endif
}
