#include "BuildEditorWidget.h"
#include "../NodeEditor/NodeEditorWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

#include <iostream>

namespace TnkrVis
{
namespace GUI
{
namespace BuildEditor
{
	
BuildEditorWidget::BuildEditorWidget(NodeEditor::NodeEditorWidget* ne, ProjectHierarchy::ProjectHierarchyWidget* phw)
{
	nodeEditorWidget = ne;
	projectHierarchyWidget = phw; 

    //Set up panel
    setWindowTitle("Build And Run"); 
    setMinimumWidth(250);
    setFeatures(QDockWidget::DockWidgetMovable); 
    scrollAreaBase = new QScrollArea();
	widgetBase = new QWidget();
	layoutBase = new QVBoxLayout();
	widgetBase->setLayout(layoutBase);
	scrollAreaBase->setWidget(widgetBase);
	scrollAreaBase->setWidgetResizable(true);
	setWidget(scrollAreaBase);
	layoutBase->setAlignment(Qt::AlignTop);

	//Add compile text box and button
	compileButton = new QPushButton("Compile"); 
	runButton = new QPushButton("Run"); 
	compileRunButtonLayout = new QHBoxLayout; 
	compileRunButtonLayout->addWidget(compileButton); 
	compileRunButtonLayout->addWidget(runButton); 
	layoutBase->addLayout(compileRunButtonLayout); 

	compileJsonTextScrollArea = new QScrollArea();
	compileJsonText = new QLabel(); 
	compileJsonText->setSizePolicy(QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );
	compileJsonText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	compileJsonTextScrollArea->setWidget(compileJsonText);
	layoutBase->addWidget(compileJsonTextScrollArea); 
	
	//Connections
	connect(compileButton, &QPushButton::clicked, [this](){

		nodeEditorWidget->SerializeToComputeGraph(&serializedComputeGraph);
		std::string str = serializedComputeGraph.ToString(); 
		compileJsonText->setText( QString::fromStdString(str) ); 
		compileJsonText->adjustSize(); 
	});
}

}	
}
}