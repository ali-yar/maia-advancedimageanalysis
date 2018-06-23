#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vdetector{}
{
    ui->setupUi(this);
    initApp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initApp()
{
    ui->actionLoad_Default_Params->trigger();
}

void MainWindow::on_actionOpen_triggered()
{
    QString dirImages = QFileDialog::getExistingDirectory(this, "Select IMAGES folder", QString(),
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QString dirMasks = QFileDialog::getExistingDirectory(this, "Select MASKS folder", QString(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QString dirTruths = QFileDialog::getExistingDirectory(this, "Select GROUND TRUTH folder", QString(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dirImages.data()->isNull() || dirMasks.data()->isNull() || dirTruths.data()->isNull())
        return ;

    ui->listWidget_images->clear();
    ui->label_accuracy->setText(QString::fromStdString("00.00 %"));

    QDir directory(dirImages);
    QStringList formats = QStringList() << "*.png" << "*.tif" << "*.bmp" << "*.jpg" << "*.jpeg" << "*.ppm"
                                        << "*.PNG" << "*.TIF" << "*.BMP" << "*.JPG" << "*.JPEG" << "*.PPM";
    QStringList images = directory.entryList(formats,QDir::Files);
    foreach(QString filename, images) {
        ui->listWidget_images->addItem(filename);
    }

    // load dataset images
    std::vector<cv::Mat> imgs = Utils::getImagesInFolder(dirImages.toStdString(), "*");
    std::vector<cv::Mat> masks  = Utils::getImagesInFolder(dirMasks.toStdString(), "*", true);
    std::vector<cv::Mat> truths = Utils::getImagesInFolder(dirTruths.toStdString(), "*", true);

    // populate the object
    vdetector.setDataset(imgs, masks, truths);
}

void MainWindow::on_pushButton_process_clicked()
{
    // get index of selected item
    int i = ui->listWidget_images->currentRow();

    if (i<0) { return; }

    // get corresponding image
    cv::Mat image = vdetector.getImage(i);
    cv::Mat mask = vdetector.getMask(i);

    // set params
    setParams();

    // segment
    cv::Mat result;
    vdetector.segment(image, mask, result);


    // display
    display(image, ui->imageLabel_ori, true);
    display(result, ui->imageLabel_segmented, false);
}

void MainWindow::setParams()
{
    int tmp;
    int numKernels = 12;
    int medianSize = ui->spinBox_median->value();
    int threshold = ui->spinBox_threshold->value();
    float gamma = ui->doubleSpinBox_gamma->value();
    int nlMeansDnoising_h = ui->spinBox_nlH->value();
    int nlMeansDnoising_templateWinSize = ui->spinBox_nlTemplate->value();
    int nlMeansDnoising_searchWinSize = ui->spinBox_nlSearch->value();
    double clahe_limit = ui->doubleSpinBox_claheLimit->value();
    tmp = ui->spinBox_claheSize->value();
    cv::Size clahe_size(tmp,tmp);
    tmp = ui->spinBox_trim->value();
    cv::Size contourTrimSize(tmp,tmp);

    vdetector.setParams(numKernels, medianSize, threshold, gamma, nlMeansDnoising_h,
                        nlMeansDnoising_templateWinSize, nlMeansDnoising_searchWinSize,
                        clahe_limit, clahe_size, contourTrimSize);
}

void MainWindow::display(const cv::Mat& im, QLabel *label, bool isColor)
{
    int W = ui->imageLabel_ori->minimumWidth();
    int H = ui->imageLabel_ori->minimumHeight();
    cv::Mat img = im.clone();
    cv::resize(img,img,cv::Size(W,H));
    if (isColor) {
        cv::cvtColor(img, img, CV_BGR2RGB);
        label->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
    } else {
        label->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8)));
    }
}

void MainWindow::on_actionDRIVE_Params_triggered()
{
    int tmp;
    int numKernels = 12;
    int medianSize = 33; ui->spinBox_median->setValue(medianSize);
    int threshold = 0; ui->spinBox_threshold->setValue(threshold);
    float gamma = .8; ui->doubleSpinBox_gamma->setValue(gamma);
    int nlMeansDnoising_h = 3; ui->spinBox_nlH->setValue(nlMeansDnoising_h);
    int nlMeansDnoising_templateWinSize = 7; ui->spinBox_nlTemplate->setValue(nlMeansDnoising_templateWinSize);
    int nlMeansDnoising_searchWinSize = 7; ui->spinBox_nlSearch->setValue(nlMeansDnoising_searchWinSize);
    double clahe_limit = 5.0; ui->doubleSpinBox_claheLimit->setValue(clahe_limit);
    tmp = 3;
    cv::Size clahe_size(tmp,tmp); ui->spinBox_claheSize->setValue(tmp);
    tmp = 1;
    cv::Size contourTrimSize(tmp,tmp); ui->spinBox_trim->setValue(tmp);

    vdetector.setParams(numKernels, medianSize, threshold, gamma, nlMeansDnoising_h,
                        nlMeansDnoising_templateWinSize, nlMeansDnoising_searchWinSize,
                        clahe_limit, clahe_size, contourTrimSize);
}

void MainWindow::on_actionSTARE_Params_triggered()
{
    int tmp;
    int numKernels = 12;
    int medianSize = 27; ui->spinBox_median->setValue(medianSize);
    int threshold = 0; ui->spinBox_threshold->setValue(threshold);
    float gamma = .2; ui->doubleSpinBox_gamma->setValue(gamma);
    int nlMeansDnoising_h = 1; ui->spinBox_nlH->setValue(nlMeansDnoising_h);
    int nlMeansDnoising_templateWinSize = 2; ui->spinBox_nlTemplate->setValue(nlMeansDnoising_templateWinSize);
    int nlMeansDnoising_searchWinSize = 12; ui->spinBox_nlSearch->setValue(nlMeansDnoising_searchWinSize);
    double clahe_limit = 7.0; ui->doubleSpinBox_claheLimit->setValue(clahe_limit);
    tmp = 28;
    cv::Size clahe_size(tmp,tmp); ui->spinBox_claheSize->setValue(tmp);
    tmp = 16;
    cv::Size contourTrimSize(tmp,tmp); ui->spinBox_trim->setValue(tmp);

    vdetector.setParams(numKernels, medianSize, threshold, gamma, nlMeansDnoising_h,
                        nlMeansDnoising_templateWinSize, nlMeansDnoising_searchWinSize,
                        clahe_limit, clahe_size, contourTrimSize);
}

void MainWindow::on_actionCHASE_Params_triggered()
{
    int tmp;
    int numKernels = 12;
    int medianSize = 49; ui->spinBox_median->setValue(medianSize);
    int threshold = 0; ui->spinBox_threshold->setValue(threshold);
    float gamma = .3; ui->doubleSpinBox_gamma->setValue(gamma);
    int nlMeansDnoising_h = 3; ui->spinBox_nlH->setValue(nlMeansDnoising_h);
    int nlMeansDnoising_templateWinSize = 5; ui->spinBox_nlTemplate->setValue(nlMeansDnoising_templateWinSize);
    int nlMeansDnoising_searchWinSize = 18; ui->spinBox_nlSearch->setValue(nlMeansDnoising_searchWinSize);
    double clahe_limit = 2.0; ui->doubleSpinBox_claheLimit->setValue(clahe_limit);
    tmp = 11;
    cv::Size clahe_size(tmp,tmp); ui->spinBox_claheSize->setValue(tmp);
    tmp = 1;
    cv::Size contourTrimSize(tmp,tmp); ui->spinBox_trim->setValue(tmp);

    vdetector.setParams(numKernels, medianSize, threshold, gamma, nlMeansDnoising_h,
                        nlMeansDnoising_templateWinSize, nlMeansDnoising_searchWinSize,
                        clahe_limit, clahe_size, contourTrimSize);
}

void MainWindow::on_pushButton_accuracy_clicked()
{
    if (ui->listWidget_images->count() == 0) { return; }

    // Run segmentation process
    vdetector();

    // Obtain segmented images
    std::vector<cv::Mat> results = vdetector.getSegmentedList();

    // Compute accuracy
    double acc = Utils::accuracy(results, vdetector.getTruths(), vdetector.getMasks());
    char res[5];
    sprintf(res, "%.2f", acc * 100);

    ui->label_accuracy->setText(QString::fromUtf8(res).append(" %"));
}

void MainWindow::on_actionLoad_Default_Params_triggered()
{
    int tmp;
    int numKernels = 12;
    int medianSize = 33; ui->spinBox_median->setValue(medianSize);
    int threshold = 0; ui->spinBox_threshold->setValue(threshold);
    float gamma = .5; ui->doubleSpinBox_gamma->setValue(gamma);
    int nlMeansDnoising_h = 2; ui->spinBox_nlH->setValue(nlMeansDnoising_h);
    int nlMeansDnoising_templateWinSize = 7; ui->spinBox_nlTemplate->setValue(nlMeansDnoising_templateWinSize);
    int nlMeansDnoising_searchWinSize = 7; ui->spinBox_nlSearch->setValue(nlMeansDnoising_searchWinSize);
    double clahe_limit = 2.0; ui->doubleSpinBox_claheLimit->setValue(clahe_limit);
    tmp = 5;
    cv::Size clahe_size(tmp,tmp); ui->spinBox_claheSize->setValue(tmp);
    tmp = 3;
    cv::Size contourTrimSize(tmp,tmp); ui->spinBox_trim->setValue(tmp);

    vdetector.setParams(numKernels, medianSize, threshold, gamma, nlMeansDnoising_h,
                        nlMeansDnoising_templateWinSize, nlMeansDnoising_searchWinSize,
                        clahe_limit, clahe_size, contourTrimSize);
}

void MainWindow::on_pushButton_opticdisc_clicked()
{
    // get index of selected item
    int i = ui->listWidget_images->currentRow();

    if (i<0) { return; }

    // get corresponding image
    cv::Mat image = vdetector.getImage(i);
    cv::Mat truth = vdetector.getTruth(i);

    // find center of disc
    cv::Point2i center = Utils::findOpticDisk(truth);
    cv::circle(truth,center,40,125,3);

    // display
    display(image, ui->imageLabel_ori, true);
    display(truth, ui->imageLabel_segmented, false);
}
