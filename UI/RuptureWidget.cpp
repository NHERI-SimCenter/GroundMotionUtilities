#include "RuptureWidget.h"
#include "HBoxFormLayout.h"

RuptureWidget::RuptureWidget(PointSourceRupture& rupture, QWidget *parent, Qt::Orientation orientation) : QWidget(parent), m_eqRupture(rupture)
{
    //We use a grid layout for the Rupture widget
    QGridLayout* layout = new QGridLayout(this);

    this->m_siteGroupBox = new QGroupBox(tr("Earthquake Rupture"));

    this->m_magnitudeBox = new QDoubleSpinBox(this->m_siteGroupBox);
    this->m_magnitudeBox->setRange(4.5, 9.0);
    this->m_magnitudeBox->setDecimals(3);
    this->m_magnitudeBox->setSingleStep(0.001);

    QVBoxLayout* siteBoxLayout = new QVBoxLayout(this->m_siteGroupBox);
    QFormLayout* magnitudeFormLayout = new QFormLayout();
    magnitudeFormLayout->addRow(tr("Magnitude:"), this->m_magnitudeBox);
    if(orientation == Qt::Horizontal)
    {
        magnitudeFormLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    }
    this->m_locationGroupBox = new QGroupBox(tr("Location"));
    this->m_locationGroupBox->setFlat(true);

    QVBoxLayout* locationLayout = new QVBoxLayout(this->m_locationGroupBox);
    this->m_latitudeBox = new QDoubleSpinBox(this->m_locationGroupBox);
    this->m_latitudeBox->setRange(-90.0, 90.0);
    this->m_latitudeBox->setDecimals(3);
    this->m_latitudeBox->setSingleStep(0.001);

    this->m_longitudeBox = new QDoubleSpinBox(this->m_locationGroupBox);
    this->m_longitudeBox->setRange(-180.0, 180.0);
    this->m_longitudeBox->setDecimals(3);
    this->m_longitudeBox->setSingleStep(0.001);

    this->m_depthBox = new QDoubleSpinBox(this->m_locationGroupBox);
    this->m_depthBox->setRange(0.0, 100.0);
    this->m_depthBox->setDecimals(3);
    this->m_depthBox->setSingleStep(0.001);

    if(orientation == Qt::Horizontal)
    {
        HBoxFormLayout* locationFormLayout = new HBoxFormLayout();
        locationFormLayout->addField(tr("Latitude:"), this->m_latitudeBox);
        locationFormLayout->addField(tr("Longitude:"), this->m_longitudeBox);
        locationFormLayout->addField(tr("Depth:"), this->m_depthBox);
        locationLayout->addLayout(locationFormLayout);
    }
    else
    {
        QFormLayout* locationFormLayout = new QFormLayout();
        locationFormLayout->addRow(tr("Latitude:"), this->m_latitudeBox);
        locationFormLayout->addRow(tr("Longitude:"), this->m_longitudeBox);
        locationFormLayout->addRow(tr("Depth:"), this->m_depthBox);
        locationLayout->addLayout(locationFormLayout);
    }


    this->m_geometryGroupBox = new QGroupBox(tr("Geometry"));
    this->m_geometryGroupBox->setFlat(true);

    QVBoxLayout* geometryLayout = new QVBoxLayout(this->m_geometryGroupBox);
    this->m_dipBox = new QDoubleSpinBox(this->m_geometryGroupBox);
    this->m_dipBox->setRange(0.0, 90.0);
    this->m_dipBox->setDecimals(2);
    this->m_dipBox->setSingleStep(0.01);

    this->m_rakeBox = new QDoubleSpinBox(this->m_geometryGroupBox);
    this->m_rakeBox->setRange(-180.0, 180.0);
    this->m_rakeBox->setDecimals(2);
    this->m_rakeBox->setSingleStep(0.01);


    if(orientation == Qt::Horizontal)
    {
        HBoxFormLayout* geometryFormLayout = new HBoxFormLayout();
        geometryFormLayout->addField(tr("Average Dip:"), this->m_dipBox);
        geometryFormLayout->addField(tr("Average Rake:"), this->m_rakeBox);
        geometryLayout->addLayout(geometryFormLayout);
    }
    else
    {
        QFormLayout* geometryFormLayout = new QFormLayout();
        geometryFormLayout->addRow(tr("Average Dip:"), this->m_dipBox);
        geometryFormLayout->addRow(tr("Average Rake:"), this->m_rakeBox);
        geometryLayout->addLayout(geometryFormLayout);
    }

    siteBoxLayout->addLayout(magnitudeFormLayout);
    siteBoxLayout->addWidget(this->m_locationGroupBox);
    siteBoxLayout->addWidget(this->m_geometryGroupBox);

    layout->addWidget(this->m_siteGroupBox);

    this->setLayout(layout);

    //We need to set initial values
    this->m_magnitudeBox->setValue(m_eqRupture.magnitude());
    this->m_latitudeBox->setValue(this->m_eqRupture.location().latitude());
    this->m_longitudeBox->setValue(this->m_eqRupture.location().longitude());
    this->m_depthBox->setValue(this->m_eqRupture.location().depth());

    this->m_dipBox->setValue(this->m_eqRupture.averageDip());
    this->m_rakeBox->setValue(this->m_eqRupture.averageRake());

    //Now, we will connect the ui to the data
    this->setupConnections();
}

void RuptureWidget::setupConnections()
{
    //Connecting magnitude
    connect(this->m_magnitudeBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &this->m_eqRupture, &PointSourceRupture::setMagnitude);

    connect(&this->m_eqRupture, &PointSourceRupture::magnitudeChanged,
            this->m_magnitudeBox, &QDoubleSpinBox::setValue);

    //Connecting latitude
    connect(this->m_latitudeBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &this->m_eqRupture.location(), &Location::setLatitude);

    connect(&this->m_eqRupture.location(), &Location::latitudeChanged,
            this->m_latitudeBox, &QDoubleSpinBox::setValue);

    //Connecting longitude
    connect(this->m_longitudeBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &this->m_eqRupture.location(), &Location::setLongitude);

    connect(&this->m_eqRupture.location(), &Location::longitudeChanged,
            this->m_longitudeBox, &QDoubleSpinBox::setValue);

    //Connecting depth
    connect(this->m_depthBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &this->m_eqRupture.location(), &RuptureLocation::setDepth);

    connect(&this->m_eqRupture.location(), &RuptureLocation::depthChanged,
            this->m_depthBox, &QDoubleSpinBox::setValue);

    //Connecting dip
    connect(this->m_dipBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &this->m_eqRupture, &PointSourceRupture::setAverageDip);

    connect(&this->m_eqRupture, &PointSourceRupture::dipChanged,
            this->m_dipBox, &QDoubleSpinBox::setValue);

    //Connecting rake
    connect(this->m_rakeBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            &this->m_eqRupture, &PointSourceRupture::setAverageRake);

    connect(&this->m_eqRupture, &PointSourceRupture::rakeChanged,
            this->m_rakeBox, &QDoubleSpinBox::setValue);
}
