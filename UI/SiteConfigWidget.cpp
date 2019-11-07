#include "SiteConfigWidget.h"
#include <QtWidgets>
#include "SiteWidget.h"
#include "SiteGridWidget.h"

SiteConfigWidget::SiteConfigWidget(SiteConfig &siteconfig, QWidget *parent) : QWidget(parent), m_siteConfig(siteconfig)
{
    QGroupBox* siteGroupBox = new QGroupBox(tr("Site"));

    QVBoxLayout* groupLayout = new QVBoxLayout(siteGroupBox);
    //First we need to add type radio buttons
    m_typeButtonsGroup = new QButtonGroup();

    QRadioButton* siteRadioButton = new QRadioButton(tr("Single Location"));
    QRadioButton* gridRadioButton = new QRadioButton(tr("Grid of Locations"));
    m_typeButtonsGroup->addButton(siteRadioButton, 0);
    m_typeButtonsGroup->addButton(gridRadioButton, 1);

    QGroupBox* typeGroupBox = new QGroupBox("Type", this);
    typeGroupBox->setFlat(true);
    typeGroupBox->setStyleSheet("QGroupBox { font-weight: normal;}");
    QHBoxLayout* typeLayout = new QHBoxLayout(typeGroupBox);
    typeLayout->addStretch(0.1);
    typeLayout->addWidget(siteRadioButton);
    typeLayout->addWidget(gridRadioButton);
    typeLayout->addStretch(1);

    groupLayout->addWidget(typeGroupBox);

    //We will add stacked widget to switch between grid and single location
    m_stackedWidgets = new QStackedWidget(this);

    //We will add the site to the stacked widget
    SiteWidget* siteWidget = new SiteWidget(siteconfig.site());
    m_stackedWidgets->addWidget(siteWidget);
    //we will also add a grid widget
    SiteGridWidget* siteGridWidget = new SiteGridWidget(siteconfig.siteGrid());
    m_stackedWidgets->addWidget(siteGridWidget);

    groupLayout->addWidget(m_stackedWidgets);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(siteGroupBox);

    if(m_siteConfig.getType() == SiteConfig::SiteType::Single)
    {
        m_typeButtonsGroup->button(0)->setChecked(true);
        m_stackedWidgets->setCurrentIndex(0);
    }
    else
    {
        m_typeButtonsGroup->button(1)->setChecked(true);
        m_stackedWidgets->setCurrentIndex(1);
    }

    setupConnections();
}

void SiteConfigWidget::setupConnections()
{
    connect(m_typeButtonsGroup, QOverload<int>::of(&QButtonGroup::buttonReleased), [this](int id)
    {
        if(id == 0)
            m_siteConfig.setType(SiteConfig::SiteType::Single);
        else
            m_siteConfig.setType(SiteConfig::SiteType::Grid);
    });

    connect(&m_siteConfig, &SiteConfig::typeChanged, [this](SiteConfig::SiteType type)
    {
        if(type == SiteConfig::SiteType::Single)
        {
            m_typeButtonsGroup->button(0)->setChecked(true);
            m_stackedWidgets->setCurrentIndex(0);
        }
        else
        {
            m_typeButtonsGroup->button(1)->setChecked(true);
            m_stackedWidgets->setCurrentIndex(1);
        }
    });
}
