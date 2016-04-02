#include <fstream>

// CGAL headers
#include <CGAL/point_generators_2.h>

// Qt headers
#include <QtGui>
#include <QString>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QScrollBar>

// for viewportsBbox
#include <CGAL/Qt/utility.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() :
    DemosMainWindow(), s(2.0), wspd(2, 2.0)
{
  setupUi(this);

  pgi = new CGAL::Qt::PointsGraphicsItem<Point_vector>(&points);
  QObject::connect(this, SIGNAL(changed()), pgi, SLOT(modelChanged()));
  pgi->setVerticesPen(QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  scene.addItem(pgi);

  wspd_item = new WSPDGraphicsItem<Traits>(&wspd);
  QObject::connect(this, SIGNAL(changed()), wspd_item, SLOT(modelChanged()));
  wspd_item->setVerticesPen(QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  scene.addItem(wspd_item);

  gvpi = new CGAL::Qt::GraphicsViewPointInput<K>(this, &scene);
  QObject::connect(gvpi, SIGNAL(generate(CGAL::Object)),
  		   this, SLOT(processInput(CGAL::Object)));
  //
  // Manual handling of actions
  //

  QObject::connect(this->actionQuit, SIGNAL(triggered()),
       this, SLOT(close()));

  // Check two actions
  this->actionInsertPoint->setChecked(true);
  this->actionShowWspd->setChecked(true);
  this->wspd_item->setDrawWspd(true);
  this->actionShowBoundingBoxes->setChecked(false);
  this->wspd_item->setDrawBoundingBoxes(false);

  //
  // Setup the scene and the view
  //
  scene.setItemIndexMethod(QGraphicsScene::NoIndex);
  scene.setSceneRect(-100, -100, 100, 100);
  this->graphicsView->setScene(&scene);
  this->graphicsView->setMouseTracking(true);

  // Turn the vertical axis upside down
  this->graphicsView->matrix().scale(1, -1);

  // The navigation adds zooming and translation functionality to the
  // QGraphicsView
  this->addNavigation(this->graphicsView);

  this->setupStatusBar();
  this->setupOptionsMenu();
  //this->addAboutDemo(":/cgal/help/about_Delaunay_triangulation_2.html");
  this->addAboutCGAL();
  this->setupExportSVG(actionExport_SVG, graphicsView);

  this->addRecentFiles(this->menuFile, this->actionQuit);
  connect(this, SIGNAL(openRecentFile(QString)),
  this, SLOT(open(QString)));
}

MainWindow::~MainWindow()
{
  delete pgi;
  delete wspd_item;
  delete gvpi;
}

void MainWindow::processInput(CGAL::Object o)
{
  Point_2 p;
  if(CGAL::assign(p, o)){
    if(std::find(points.begin(), points.end(), p) == points.end()) {
      points.push_back(p);
      reset_wspd();
      Q_EMIT( changed());
    }
  }
}

void MainWindow::on_actionInsertPoint_toggled(bool checked)
{
  if(checked){
    scene.installEventFilter(gvpi);
    scene.installEventFilter(pgi);
  } else {
    scene.removeEventFilter(gvpi);
    scene.removeEventFilter(pgi);
  }
}

void MainWindow::on_actionShowWspd_toggled(bool checked)
{
  wspd_item->setDrawWspd(checked);
}


void MainWindow::on_actionShowBoundingBoxes_toggled(bool checked)
{
  wspd_item->setDrawBoundingBoxes(checked);
}

void MainWindow::on_actionClear_triggered()
{
  points.clear();
  reset_wspd();
  Q_EMIT( changed());
}


void MainWindow::on_actionInsertRandomPoints_triggered()
{
  QRectF rect = CGAL::Qt::viewportsBbox(&scene);
  Iso_rectangle_2 isor = convert(rect);
  CGAL::Random_points_in_iso_rectangle_2<Point_2> pg((isor.min)(), (isor.max)());

  const int number_of_points =
    QInputDialog::getInt(this,
                             tr("Number of random points"),
                             tr("Enter number of random points"), 10, 0);

  // wait cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  points.reserve(points.size() + number_of_points);
  for(int i = 0; i < number_of_points; ++i){
    points.push_back(*pg++);
  }
  reset_wspd();
  // default cursor
  QApplication::setOverrideCursor(Qt::ArrowCursor);
  Q_EMIT( changed());
}

void MainWindow::on_actionSetSeparationRatio_triggered() {
  s =
    QInputDialog::getDouble(this,
                             tr("Separation ratio"),
                             tr("Enter the separation ratio"), s, 0.0);
  wspd.separation_ratio(s);
  Q_EMIT( changed());
}

void MainWindow::on_actionLoadPoints_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Open Points file"),
						  ".");
  if(! fileName.isEmpty()){
    open(fileName);
  }
}

void MainWindow::open(QString fileName)
{
  // wait cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  std::ifstream ifs(qPrintable(fileName));

  points.clear();
  Point_2 p;
  while(ifs >> p) {
    points.push_back(p);
  }
  reset_wspd();

  actionRecenter->trigger();

  // default cursor
  QApplication::restoreOverrideCursor();
  this->addToRecentFiles(fileName);
  actionRecenter->trigger();
  Q_EMIT(changed());
}

void MainWindow::on_actionSavePoints_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this,
						  tr("Save points"),
						  ".");
  if(! fileName.isEmpty()){
    std::ofstream ofs(qPrintable(fileName));
    for(Point_vector::iterator it = points.begin(); it!= points.end(); ++it)
    {
      ofs << *it << std::endl;
    }
  }
}

void MainWindow::on_actionRecenter_triggered()
{
  this->graphicsView->setSceneRect(wspd_item->boundingRect());
  this->graphicsView->fitInView(wspd_item->boundingRect(), Qt::KeepAspectRatio);
}

void
MainWindow::reset_wspd()
{
  wspd.set(2, points.begin(), points.end());
}
