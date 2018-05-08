#include "SiteGridWidget.h"
#include "HBoxFormLayout.h"

SiteGridWidget::SiteGridWidget(SiteGrid& siteGrid, QWidget *parent) : QWidget(parent), m_siteGrid(siteGrid)
{
    //We use a grid layout for the site widget
    QGridLayout* layout = new QGridLayout(this);

    QGroupBox* latitudeGroupBox = new QGroupBox(tr("Latitude"));
    latitudeGroupBox->setFlat(true);
    latitudeGroupBox->setStyleSheet("QGroupBox { font-weight: normal;}");
    QVBoxLayout* latitudeLayout = new QVBoxLayout(latitudeGroupBox);

    m_minLatitudeBox = new QDoubleSpinBox(latitudeGroupBox);
    m_minLatitudeBox->setRange(-90.0, 90.0);
    m_minLatitudeBox->setDecimals(3);
    m_minLatitudeBox->setSingleStep(0.001);
    m_minLatitudeBox->setValue(m_siteGrid.latitude().min());
    HBoxFormLayout* latitudeFormLayout = new HBoxFormLayout();
    latitudeFormLayout->addField(tr("Min:"), this->m_minLatitudeBox);

    m_maxLatitudeBox = new QDoubleSpinBox(latitudeGroupBox);
    m_maxLatitudeBox->setRange(-90.0, 90.0);
    m_maxLatitudeBox->setDecimals(3);
    m_maxLatitudeBox->setSingleStep(0.001);
    m_maxLatitudeBox->setValue(m_siteGrid.latitude().max());
    latitudeFormLayout->addField(tr("Max:"), this->m_maxLatitudeBox);
    latitudeLayout->addLayout(latitudeFormLayout);

    m_latitudeDivBox = new QSpinBox(latitudeGroupBox);
    m_latitudeDivBox->setRange(1, 500);
    m_latitudeDivBox->setValue(m_siteGrid.latitude().divisions());
    latitudeFormLayout->addField(tr("Divisions:"), this->m_latitudeDivBox);

    QGroupBox* longitudeGroupBox = new QGroupBox(tr("Longitude"));
    longitudeGroupBox->setFlat(true);
    longitudeGroupBox->setStyleSheet("QGroupBox { font-weight: normal;}");
    QVBoxLayout* longitudeLayout = new QVBoxLayout(longitudeGroupBox);

    m_minLongitudeBox = new QDoubleSpinBox(longitudeGroupBox);
    m_minLongitudeBox->setRange(-180.0, 180.0);
    m_minLongitudeBox->setDecimals(3);
    m_minLongitudeBox->setSingleStep(0.001);
    m_minLongitudeBox->setValue(m_siteGrid.longitude().min());
    HBoxFormLayout* longitudeFormLayout = new HBoxFormLayout();
    longitudeFormLayout->addField(tr("Min:"), this->m_minLongitudeBox);

    m_maxLongitudeBox = new QDoubleSpinBox(longitudeGroupBox);
    m_maxLongitudeBox->setRange(-180.0, 180.0);
    m_maxLongitudeBox->setDecimals(3);
    m_maxLongitudeBox->setSingleStep(0.001);
    m_maxLongitudeBox->setValue(m_siteGrid.longitude().max());
    longitudeFormLayout->addField(tr("Max:"), this->m_maxLongitudeBox);
    longitudeLayout->addLayout(longitudeFormLayout);

    m_longitudeDivBox = new QSpinBox(longitudeGroupBox);
    m_longitudeDivBox->setRange(1, 500);
    m_longitudeDivBox->setValue(m_siteGrid.longitude().divisions());
    longitudeFormLayout->addField(tr("Divisions:"), this->m_longitudeDivBox);

//    m_minLongitudeBox = new QDoubleSpinBox(this->m_locationGroupBox);
//    m_minLongitudeBox->setRange(-180.0, 180.0);
//    m_minLongitudeBox->setDecimals(3);
//    m_minLongitudeBox->setSingleStep(0.001);
//    m_minLongitudeBox->setValue(this->m_site.location().longitude());




//    QWidget* siteGroupWidget = new QWidget(this);
//    QVBoxLayout* siteBoxLayout = new QVBoxLayout(siteGroupWidget);
//    siteBoxLayout->addWidget(this->m_locationGroupBox);

    layout->addWidget(latitudeGroupBox);
    layout->addWidget(longitudeGroupBox);

    layout->setContentsMargins(0, 0, 0, 0);

    //Now we need to setup the connections
    setupConnections();
}

void SiteGridWidget::setupConnections()
{
    //Connecting Latitude
    connect(this->m_minLatitudeBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_siteGrid.latitude(),
            &GridDivision::setMin);

    connect(this->m_maxLatitudeBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_siteGrid.latitude(),
            &GridDivision::setMax);

    connect(this->m_latitudeDivBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            &m_siteGrid.latitude(),
            &GridDivision::setDivisions);

    connect(&m_siteGrid.latitude(),
            &GridDivision::minChanged,
            this->m_minLatitudeBox,
            &QDoubleSpinBox::setValue);

    connect(&m_siteGrid.latitude(),
            &GridDivision::maxChanged,
            this->m_maxLatitudeBox,
            &QDoubleSpinBox::setValue);

    connect(&m_siteGrid.latitude(),
            &GridDivision::divisionsChanged,
            this->m_latitudeDivBox,
            &QSpinBox::setValue);

    //Connecting Longitude
    connect(this->m_minLongitudeBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_siteGrid.longitude(),
            &GridDivision::setMin);

    connect(this->m_maxLongitudeBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &m_siteGrid.longitude(),
            &GridDivision::setMax);

    connect(this->m_longitudeDivBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            &m_siteGrid.longitude(),
            &GridDivision::setDivisions);

    connect(&m_siteGrid.longitude(),
            &GridDivision::minChanged,
            this->m_minLongitudeBox,
            &QDoubleSpinBox::setValue);

    connect(&m_siteGrid.longitude(),
            &GridDivision::maxChanged,
            this->m_maxLongitudeBox,
            &QDoubleSpinBox::setValue);

    connect(&m_siteGrid.longitude(),
            &GridDivision::divisionsChanged,
            this->m_longitudeDivBox,
            &QSpinBox::setValue);
}
