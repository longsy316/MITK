/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$ 
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

//Mitk
#include "mitkDataNode.h"


// Qmitk
#include "UltrasoundSupport.h"
#include <QTimer>

// Qt
#include <QMessageBox>

// Ultrasound
#include "mitkUSDevice.h"

const std::string UltrasoundSupport::VIEW_ID = "org.mitk.views.ultrasoundsupport";

void UltrasoundSupport::SetFocus()
{
  m_Controls.m_AddDevice->setFocus();
}

void UltrasoundSupport::CreateQtPartControl( QWidget *parent )
{
  m_Timer = new QTimer(this);

  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi( parent );
  connect( m_Controls.m_AddDevice, SIGNAL(clicked()), this, SLOT(OnClickedAddNewDevice()) ); // Change Widget Visibilities
  connect( m_Controls.m_AddDevice, SIGNAL(clicked()), this->m_Controls.m_NewVideoDeviceWidget, SLOT(CreateNewDevice()) ); // Init NewDeviceWidget
  connect( m_Controls.m_NewVideoDeviceWidget, SIGNAL(Finished()), this, SLOT(OnNewDeviceWidgetDone()) ); // After NewDeviceWidget finished editing
  connect( m_Controls.m_BtnView, SIGNAL(clicked()), this, SLOT(OnClickedViewDevice()) ); 
  connect( m_Timer, SIGNAL(timeout()), this, SLOT(DisplayImage()));
  //connect (m_Controls.m_ActiveVideoDevices, SIGNAL())
  
  // Initializations
  m_Controls.m_NewVideoDeviceWidget->setVisible(false);
  std::string filter = "(&(" + mitk::ServiceConstants::OBJECTCLASS() + "=" + "org.mitk.services.UltrasoundDevice)(IsActive=true))";
  m_Controls.m_ActiveVideoDevices->Initialize(filter);
  
  m_Node = mitk::DataNode::New();
  m_Node->SetName("US Image Stream");
  this->GetDataStorage()->Add(m_Node);
}

void UltrasoundSupport::OnClickedAddNewDevice()
{
  MITK_INFO << "USSUPPORT: OnClickedAddNewDevice()"; 
  m_Controls.m_NewVideoDeviceWidget->setVisible(true);
  m_Controls.m_DeviceManagerWidget->setVisible(false);
  m_Controls.m_AddDevice->setVisible(false);
  m_Controls.m_Headline->setText("Add New Device:");
}

void UltrasoundSupport::DisplayImage()
{
   MITK_INFO << "USSUPPORT: DisplayImage()"; 
  //QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  // if (nodes.empty()) return;

  m_Device->UpdateOutputData(0);
  mitk::USImage::Pointer image = m_Device->GetOutput();
  //m_Node->Initialize();
  m_Node->SetData(image);
  this->RequestRenderWindowUpdate();
  /* int i;
  for (i = 1; i < 10; i++)
  {
    device->Update();
    if ((i % 3) == 0 )
    {
      node = mitk::DataNode::New();
      image = device->GetOutput();
      node->SetData(image);
      std::ostringstream oss;
      oss << "US-Image " << i;
      node->SetName(oss.str());
      this->GetDataStorage()->Add(node);
    }
  }*/



  

  //// here we have a valid mitk::DataNode

  //// a node itself is not very useful, we need its data item (the image)
  //mitk::BaseData* data = node->GetData();
  //if (data)
  //{
  //  // test if this data item is an image or not (could also be a surface or something totally different)
  //  mitk::Image* image = dynamic_cast<mitk::Image*>( data );
  //  if (image)
  //  {
  //    std::stringstream message;
  //    std::string name;
  //    message << "Performing image processing for image ";
  //    if (node->GetName(name))
  //    {
  //      // a property called "name" was found for this DataNode
  //      message << "'" << name << "'";
  //    }
  //    message << ".";
  //    MITK_INFO << message.str();

  //    // actually do something here...

  //  }
  //}

}

void UltrasoundSupport::OnClickedViewDevice()
{
  
  MITK_INFO << "USSUPPORT: OnClickedViewDevice()"; 
  m_Device = m_Controls.m_ActiveVideoDevices->GetSelectedDevice();
  if (m_Device.IsNull()){
    m_Timer->stop();
    return;
  }

  m_Timer->start(100);
}

void UltrasoundSupport::OnNewDeviceWidgetDone()
{
  MITK_INFO << "USSUPPORT: OnNewDeviceWidgetDone()";
  m_Controls.m_NewVideoDeviceWidget->setVisible(false);
  m_Controls.m_DeviceManagerWidget->setVisible(true);
  m_Controls.m_AddDevice->setVisible(true);
  m_Controls.m_Headline->setText("Connected Devices:");
}



/** EXAMPLE CODE FOR WORKING WITH DATANODES ///

void UltrasoundSupport::DoImageProcessing()
{
  MITK_INFO << "DO SUPM";
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  if (nodes.empty()) return;

  mitk::DataNode* node = nodes.front();

  if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information( NULL, "Template", "Please load and select an image before starting image processing.");
    return;
  }

  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData* data = node->GetData();
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image* image = dynamic_cast<mitk::Image*>( data );
    if (image)
    {
      std::stringstream message;
      std::string name;
      message << "Performing image processing for image ";
      if (node->GetName(name))
      {
        // a property called "name" was found for this DataNode
        message << "'" << name << "'";
      }
      message << ".";
      MITK_INFO << message.str();

      // actually do something here...

    }
  }
}

**/
