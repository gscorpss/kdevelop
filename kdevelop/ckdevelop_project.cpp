/***************************************************************************
            project.cpp - the projectmanagment specific part of CKDevelop
                             -------------------                                         

    begin                : 28 Jul 1998                                    
    copyright            : (C) 1998 by Sandy Meier                         
    email                : smeier@rz.uni-potsdam.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#include <qprogressdialog.h>
#include <qsplitter.h>
#include <kcursor.h>
#include <kstddirs.h>
#include <kmessagebox.h>

#include "ckdevelop.h"
#include "ctoolclass.h"
#include "ckappwizard.h"
#include "cclassview.h"
#include "crealfileview.h"
#include "clogfileview.h"
#include "makeview.h"
#include "ceditwidget.h"
#include "cprjoptionsdlg.h"
#include "caddexistingfiledlg.h"
#include "cfilepropdlg.h"
#include "cnewfiledlg.h"
#include "cnewclassdlg.h"
#include "caddnewtranslationdlg.h"
#include "cmakemanualdlg.h"
#include "cgeneratenewfile.h"
#include "componentmanager.h"
#include "misc.h"


/*********************************************************************
 *                                                                   *
 *                              SLOTS                                *
 *                                                                   *
 ********************************************************************/ 

void CKDevelop::slotProjectImport()
{}

bool CKDevelop::slotProjectClose(){
    slotStatusMsg(i18n("Closing project..."));
    m_log_file_tree->storeState(m_prj);
    bool cont;
    cont = slotFileCloseAll();
    if(cont){
	if(!m_bKDevelop){
	    switchToKDevelop();
	}
	ComponentManager::self()->notifyProjectClosed(); // notify all components
	
	
	toolBar(ID_BROWSER_TOOLBAR)->clearCombo(ID_CV_TOOLBAR_CLASS_CHOICE);
	toolBar(ID_BROWSER_TOOLBAR)->clearCombo(ID_CV_TOOLBAR_METHOD_CHOICE);
       
    
	// set m_project to false and disable all ID_s related to m_project=true	
	m_prj->writeProject();
	m_project=false;
	m_prj->valid = false;
	delete m_prj;
	m_prj = 0;
	
	
	disableCommand(ID_FILE_NEW);
	// doc menu
	disableCommand(ID_HELP_PROJECT_API);
	disableCommand(ID_HELP_USER_MANUAL);
	// build menu
	setToolMenuProcess(false);  
	disableCommand(ID_BUILD_STOP);
	disableCommand(ID_BUILD_AUTOCONF);
	disableCommand(ID_KDLG_BUILD_GENERATE);
	
	// prj menu
	disableCommand(ID_PROJECT_CLOSE);
	disableCommand(ID_PROJECT_ADD_FILE_EXIST);
	disableCommand(ID_PROJECT_ADD_NEW_TRANSLATION_FILE);
	disableCommand(ID_PROJECT_REMOVE_FILE);
	disableCommand(ID_PROJECT_NEW_CLASS);
	disableCommand(ID_PROJECT_FILE_PROPERTIES);
	disableCommand(ID_PROJECT_OPTIONS);
	disableCommand(ID_PROJECT_MAKE_DISTRIBUTION);
	
	disableCommand(ID_CV_WIZARD);
	disableCommand(ID_CV_GRAPHICAL_VIEW);
	disableCommand(ID_CV_TOOLBAR_CLASS_CHOICE);
  	disableCommand(ID_CV_TOOLBAR_METHOD_CHOICE);
	
	m_file_open_popup->clear();
	m_file_open_list.clear();
    }
    
    slotStatusMsg(i18n("Ready."));
    refreshTrees();
    
    if (!cont)
	{
	    setMainCaption();
	}
    
    return cont; // false if pressed cancel
}

void CKDevelop::slotProjectAddNewFile(){
  newFile(true);
}

void CKDevelop::slotAddExistingFiles(){
  QString temp_template;
  QFileInfo fi;
  CGenerateNewFile genfile;
  bool copy = false;
  ProjectFileType type = DATA;
  bool new_subdir=false; // if a new subdir was added to the project, we must do a rebuildmakefiles
  QString token;
  QStrList files;
  QString str_files = m_add_dlg->source_edit->text();
  StringTokenizer str_token;
    
  str_token.tokenize(str_files,",");
  while(str_token.hasMoreTokens()){
    token = str_token.nextToken();
    files.append(token);
  }
  QString dest = m_add_dlg->destination_edit->text();
  if(dest.right(1) != "/"){ // I hope it works now -Sandy
    dest = dest + "/";
  }
  QString source_name;
  QString dest_name ;
  QString file;
  QFileInfo file_info;
  int i=files.count();
    
  QProgressDialog progress( i18n("Copying files..."),0, i, this,"",true );
  progress.setCaption(i18n("please wait..."));
  progress.show();
    
  i=0;
  progress.setProgress( i);

  for(file = files.first(); file !=0;file = files.next()){
    i++;
    copy = false;
    progress.setProgress( i );
    if (!QFile::exists((const char*)file)) {
        KMessageBox::sorry(this, i18n("File %1does not exist!").arg(file));
        continue;
    }
    file_info.setFile(file);
    source_name = file_info.fileName();
    dest_name = dest + source_name;
   
    // Fetch the type of the file
    type = CProject::getType( dest_name );


    if(file == dest_name) {
      copy = false;
      KShellProcess process("/bin/sh");
      process << "cat"; // copy is your friend :-) ...cat, too

      if (m_add_dlg->isTemplateChecked())
      {
       if (CProject::getType(file)==CPP_HEADER)
        {
         temp_template = genfile.genHeaderFile(locate("appdata","temp_template"), m_prj,fi.fileName());
         process << temp_template;
        }
        else if (CProject::getType(file)==CPP_SOURCE)
              {
               temp_template = genfile.genCPPFile(locate("appdata","temp_template"), m_prj, fi.fileName());
               process << temp_template;
              }
      }
      process << file;
      process << ">";

      process << "temp.tmp";
      process.start(KProcess::Block,KProcess::AllOutput); // blocked because it is important 
      process.clearArguments();
      process << "mv";
      process << "temp.tmp";
      process << dest_name;
      process.start(KProcess::Block,KProcess::AllOutput);
    }
    else      
    if(QFile::exists(dest_name)){
        int result=KMessageBox::warningYesNoCancel(this,
                                                   i18n("The file\n\n%1\n\n"
                                                        "already exists.\nDo you want overwrite the old one?").arg(source_name));
      if(result==KMessageBox::Yes)
        copy = true;
      if(result==KMessageBox::No)
        copy = false;
      if(result==3){
        setCursor( KCursor::arrowCursor() );
        break;;
      }
    }
    else {
      copy = true;
    }
      
    if(copy){
      KShellProcess process("/bin/sh");
      process << "cat"; // copy is your friend :-) ...cat, too

      fi.setFile(file);

      if (m_add_dlg->isTemplateChecked())
      {
       if (CProject::getType(file)==CPP_HEADER)
        {
         temp_template = genfile.genHeaderFile(locate("appdata","temp_template"), m_prj,fi.fileName());
         process << temp_template;
        }
        else if (CProject::getType(file)==CPP_SOURCE)
              {
               temp_template = genfile.genCPPFile(locate("appdata","temp_template"), m_prj, fi.fileName());
               process << temp_template;
              }
      }
      process << file;
      process << ">";

      process << dest+fi.fileName();
      process.start(KProcess::Block,KProcess::AllOutput); // blocked because it is important  
    }
    new_subdir = addFileToProject(dest_name,type,false) || new_subdir; // no refresh
  }
  progress.setProgress( files.count() );
  // if (type != DATA)               // don't load data files (has to be tested if wanted)
   switchToFile(dest_name);
  refreshTrees();
    
  if(new_subdir){
    newSubDir();
  }

  delete m_add_dlg;
}

void CKDevelop::slotProjectAddExistingFiles(){
  m_add_dlg = new CAddExistingFileDlg(this,"test",m_prj);

  m_add_dlg->destination_edit->setText(m_prj->getProjectDir()+ m_prj->getSubDir());

  if(m_add_dlg->exec()){
    QTimer::singleShot(100,this,SLOT(slotAddExistingFiles()));
  }
}

void CKDevelop::slotProjectRemoveFile(){
    QString name = m_log_file_tree->currentItem()->text(0);
    delFileFromProject(name);
}

void CKDevelop::slotProjectOptions(){
  CPrjOptionsDlg prjdlg(this,"optdialog",m_prj);
  
  QString args=m_prj->getConfigureArgs();

  if(prjdlg.exec()){
    if (prjdlg.needConfigureInUpdate()){
      m_prj->updateConfigureIn();
      KMessageBox::information(0, i18n("You have modified the projectversion.\n"
                                       "We will regenerate all Makefiles now."));
      setToolMenuProcess(false);
      slotStatusMsg(i18n("Running automake/autoconf and configure..."));
      showOutputView(true);
      m_messages_widget->prepareJob(m_prj->getProjectDir());
      (*m_messages_widget) << m_make_cmd << " -f Makefile.dist  && ";
      (*m_messages_widget) << m_prj->getCompilationEnvironment();
      (*m_messages_widget) << " ./configure" << args;

      m_messages_widget->startJob();
      return;
    }
    if(prjdlg.needMakefileUpdate()){
      m_prj->updateMakefilesAm();
      setToolMenuProcess(false);
      slotStatusMsg(i18n("Running configure..."));
      showOutputView(true);
      m_messages_widget->prepareJob(m_prj->getProjectDir());
      (*m_messages_widget) << m_prj->getCompilationEnvironment();
      (*m_messages_widget) << " ./configure "  << args;
      m_messages_widget->startJob();
    }
  }
  
}
void CKDevelop::slotProjectNewClass(){
  CNewClassDlg* dlg = new CNewClassDlg(this,"newclass",m_prj);
  if(dlg->exec()){
    QString source_file=dlg->getImplFile() ;
    QString header_file=dlg->getHeaderFile();
    switchToFile(source_file);
    switchToFile(header_file);

    QFileInfo header_info(header_file);
    QFileInfo source_info(source_file);
    QString header_relname = m_prj->getSubDir() + header_info.fileName();
    QString source_relname = m_prj->getSubDir() + source_info.fileName();
    TFileInfo file_info;
    file_info.rel_name = source_relname;
    file_info.type = CPP_SOURCE;
    file_info.dist = true;
    file_info.install = false;
    m_prj->addFileToProject(source_relname, file_info);
    ComponentManager::self()->notifyAddedFileToProject(source_relname);
    
    file_info.rel_name = header_relname;
    file_info.type = CPP_HEADER;
    file_info.dist = true;
    file_info.install = false;
    m_prj->addFileToProject(header_relname, file_info);
    ComponentManager::self()->notifyAddedFileToProject(header_relname);
   
    m_prj->updateMakefilesAm();
  }
}

void CKDevelop::slotProjectFileProperties(){
  CFilePropDlg dlg(this,"DLG",m_prj);
  dlg.show();
}

void CKDevelop::slotShowFileProperties(QString rel_name){
  CFilePropDlg dlg(this,"DLG",m_prj,rel_name);
  dlg.show();
}

void CKDevelop::slotProjectOpen(){
  slotStatusMsg(i18n("Opening project..."));
  KURL filename = KFileDialog::getOpenURL(0,"*.kdevprj",this);
  if( !filename.isLocalFile() )
  {
    KMessageBox::sorry( 0L, i18n( "Only local files are supported" ) );
    return;
  }
  if (filename.isEmpty())
      return;
  
  if (m_project && !slotProjectClose())
      return;
 
  QFileInfo info(filename.path());
  if (info.isFile()){
    if(!(readProjectFile(filename.path()))){

    KMessageBox::sorry(0, i18n("This is a Project-File from KDevelop 0.1\nSorry,but it's incompatible with KDevelop >= 0.2.\n"
                               "Please use only new generated projects!"));
    }
    ComponentManager::self()->notifyProjectOpened(m_prj);
    slotStatusMsg(i18n("Ready."));
  }	
  
}

void CKDevelop::slotProjectOpenRecent(int id_)
{
  slotProjectOpenCmdl(m_recent_projects.at(id_));
}

void CKDevelop::slotProjectOpenCmdl(QString prjname){
  if (m_project && !slotProjectClose())
      return;
 
  prjname.replace(QRegExp("file:"),"");
  
  QFileInfo info(prjname);
  if (info.isFile()){
    if(!(readProjectFile(prjname))){
      KMessageBox::sorry(0, "This is a Project-File from KDevelop 0.1\nSorry,but it's incompatible with KDevelop >= 0.2.\n"
                         "Please use only new generated projects!");
    }
    ComponentManager::self()->notifyProjectOpened(m_prj);
    slotStatusMsg(i18n("Ready."));
  }	
}

void CKDevelop::slotProjectNewAppl(){
  QString old_project="";
  if(!CToolClass::searchProgram("perl")){
    return;
  }
  if(!CToolClass::searchProgram("autoconf")){
    return;
  }
  if(!CToolClass::searchProgram("automake")){
    return;
  }
  if(m_project){
    old_project = m_prj->getProjectFile();
    if(!slotProjectClose()){
      return;
    }
  }
  
  slotStatusMsg(i18n("Creating a new frame application..."));
  m_config->setGroup("General Options");
  CKAppWizard kappw(this,"zutuz",m_config->readEntry("author_name",""),
		    m_config->readEntry("author_email",""));
  
  
  kappw.exec();
  QString file = kappw.getProjectFile();
  
  if(kappw.generatedProject()){
    m_config->setGroup("General Options");
    m_config->writeEntry("author_name",kappw.getAuthorName());
    m_config->writeEntry("author_email",kappw.getAuthorEmail());
    m_config->sync();
    readProjectFile(file);
    if (m_prj->getProjectType() == "normal_kde" || m_prj->getProjectType() == "mini_kde") {
      slotProjectMessages();
    }
  }
  else if (old_project != ""){ // if cancel load the old project again
    readProjectFile(old_project);
  }
  slotViewRefresh();
  //cerr << kappw->getProjectFile();
  slotStatusMsg(i18n("Ready."));
}



void CKDevelop::slotProjectAddNewTranslationFile(){
  CAddNewTranslationDlg dlg(this,0,m_prj);
  if (dlg.exec()){
    QString rel_name = "po/" + dlg.getLangFile();
    QString file = m_prj->getProjectDir() + rel_name;
    QFile nfile(file); // create a empty file
    nfile.open(IO_WriteOnly);
    nfile.close();

    TFileInfo info;
    info.rel_name = rel_name;
    info.type = PO;
    info.dist = false;
    info.install = false;
    info.install_location = "";
    (void) m_prj->addFileToProject(rel_name, info);
    ComponentManager::self()->notifyAddedFileToProject(rel_name);

    m_prj->writeProject();
    m_prj->updateMakefilesAm();
    slotProjectMessages();
  }
}
void CKDevelop::slotAddFileToProject(QString abs_filename){
  ProjectFileType type = CProject::getType( abs_filename );

  addFileToProject(abs_filename, type, false);

  // Parse the file if it's a sourcefile.
  if( type == CPP_SOURCE || type == CPP_HEADER )
    m_class_tree->addFile( abs_filename );

  m_log_file_tree->refresh( m_prj );
}

void CKDevelop::slotProjectMessages(){
  if(!CToolClass::searchProgram("xgettext")){
    return;
  }
  showOutputView(true);
  setToolMenuProcess(false);
  slotFileSaveAll();
  slotStatusMsg(i18n("Creating pot-file in /po..."));
  m_messages_widget->prepareJob(m_prj->getProjectDir() + m_prj->getSubDir());
  (*m_messages_widget) << m_make_cmd << " messages &&  cd ../po && make merge";
  m_messages_widget->startJob();
  m_beep = true;
}

void CKDevelop::slotProjectAPI(){
  if(!CToolClass::searchProgram("kdoc")){
    return;
  }
  showOutputView(true);

  setToolMenuProcess(false);
  slotFileSaveAll();
  slotStatusMsg(i18n("Creating project API-Documentation..."));
  m_messages_widget->clear();

  m_config->setGroup("Doc_Location");
  QString idx_path = m_config->readEntry("kdoc_index", KDOC_INDEXDIR);
  if (idx_path.isEmpty())
      idx_path = m_config->readEntry("doc_kde", KDELIBS_DOCDIR)
          + "/kdoc-reference";
  QString link;
  if ( QFileInfo(idx_path + "/qt.kdoc").exists() ||
       QFileInfo(idx_path + "/qt.kdoc.gz").exists() )
      link = "-lqt ";
  // This could me made a lot smarter...
  if ( QFileInfo(idx_path + "/kdecore.kdoc").exists() ||
       QFileInfo(idx_path + "/kdecore.kdoc.gz").exists() )
      link += "-lkdecore -lkdeui -lkfile -lkfmlib -lkhtml -ljscript -lkab -lkspell";

  QDir::setCurrent(m_prj->getProjectDir() + m_prj->getSubDir());
  int dirlength = QDir::currentDirPath().length()+1;

  QString sources;
  QStrList headerlist(m_prj->getHeaders());
  QStrListIterator it(headerlist);
  for (; it.current(); ++it)
      {
          QString file = it.current();
          file.remove(0, dirlength);
          sources += file;
          sources += " ";
      }

  m_messages_widget->prepareJob(m_prj->getProjectDir() + m_prj->getSubDir());
  (*m_messages_widget) << "kdoc";
  (*m_messages_widget) << "-p -d" + m_prj->getProjectDir() + m_prj->getSubDir() +  "api";
  if (!sources.isEmpty())
      (*m_messages_widget) << sources;
  if (!link.isEmpty())
      {
          (*m_messages_widget) << ("-L" + idx_path);
          (*m_messages_widget) << link;
      }

  m_messages_widget->startJob();
}


void CKDevelop::slotProjectManual(){

	CMakeManualDlg dlg(this,"tesr",m_prj->getSGMLFile());
  if(dlg.exec()){

		showOutputView(true);
		setToolMenuProcess(false);
		//  slotFileSaveAll();
		slotStatusMsg(i18n("Creating project Manual..."));
		m_messages_widget->clear();

		if((dlg.file).right(8) == ".docbook"){
			QFileInfo info(dlg.file);
			if(!CToolClass::searchProgram("db2html"))
	    	return;
			
			m_messages_widget->prepareJob(info.dirPath());
			(*m_messages_widget) << "db2html -d /usr/lib/sgml/stylesheets/kde.dsl ";
			(*m_messages_widget) << info.absFilePath();
			m_messages_widget->startJob();

		}
		else{
			bool ksgml = true;
			if(dlg.program == "sgml2html") ksgml = false;
			m_prj->setSGMLFile(dlg.file);
			CGenerateNewFile generator;
			QFileInfo info(dlg.file);

			if(ksgml){
				QString nif_file = info.dirPath() + "/" + info.baseName()+ ".nif";
				if(!QFile::exists(nif_file)){
					generator.genNifFile(nif_file);
				}
			}
			m_messages_widget->prepareJob(info.dirPath());
			if(ksgml){
				(*m_messages_widget) << "ksgml2html";
				(*m_messages_widget) << info.fileName();
				(*m_messages_widget) << "en";
			}
			else{
				(*m_messages_widget) << "sgml2html";
				(*m_messages_widget) << info.fileName();
			}
			m_messages_widget->startJob();
		}
	}
}

void CKDevelop::slotProjectMakeDistSourceTgz(){
  if(!m_view_menu->isItemChecked(ID_VIEW_OUTPUTVIEW)){
/*    QValueList<int> sizes;
    sizes << output_view_pos;
    view->setSizes(sizes);
*/
    m_dockbase_o_tab_view->show();
    m_view_menu->setItemChecked(ID_VIEW_OUTPUTVIEW,true);
/*    QRect rMainGeom= view->geometry();
    view->resize(rMainGeom.width()-1,rMainGeom.height());
    view->resize(rMainGeom.width()+1,rMainGeom.height());
*/  }

  showOutputView(true);
  setToolMenuProcess(false);
  slotFileSaveAll();
  slotStatusMsg(i18n("Running make dist..."));
  m_messages_widget->clear();
  m_messages_widget->prepareJob(m_prj->getProjectDir());
  (*m_messages_widget) << m_make_cmd << " dist";
  m_messages_widget->startJob();
  m_beep = true;
}

/*********************************************************************
 *                                                                   *
 *                          PUBLIC METHODS                           *
 *                                                                   *
 ********************************************************************/ 

/*---------------------------------------------- CKDevelop::newFile()
 * newFile()
 *   Create a new file and add it to the project.
 *
 * Parameters:
 *   add_to_project      Should the file be added to the project?
 *
 * Returns:
 *   -
 *-----------------------------------------------------------------*/
void CKDevelop::newFile(bool add_to_project){
  ProjectFileType type;
  bool new_subdir=false;
  QString complete_filename;
  CNewFileDlg dlg(this,"test",true,0,m_prj);

  dlg.setUseTemplate();
  if (add_to_project){
    dlg.setAddToProject();
  }
  if(!dlg.exec()) return; // cancel
  
  complete_filename = dlg.location() + dlg.fileName();

  // Get the filetype.
  type = CProject::getType( complete_filename );
  // load into the widget
  switchToFile(complete_filename);
  
  
  
  // add the file to the project if necessary
  if (dlg.addToProject() == true){
    new_subdir = addFileToProject(complete_filename,type);
  }
  if(new_subdir){
    newSubDir();
  }
  
}

/*-------------------------------------- CKDevelop::addFileToProject()
 * addFileToProject()
 *   Add a file with a specified type to the project.
 *
 * Parameters:
 *   complete_filename   The absolute filename.
 *   type                Type of file.
 *   refresh             If to refresh the trees.
 *
 * Returns:
 *   true                If a new subdir was added.
 *-----------------------------------------------------------------*/
bool CKDevelop::addFileToProject(QString complete_filename,
                                 ProjectFileType type,
                                 bool refresh)
{
  bool new_subdir = false;
  QString rel_name = complete_filename;
  
  // normalize it a little bit
  rel_name.replace(QRegExp("///"),"/"); // remove ///
  rel_name.replace(QRegExp("//"),"/"); // remove //
		   
  rel_name.remove(0,m_prj->getProjectDir().length());
  //  rel_name.replace(QRegExp(m_prj->getProjectDir()),"");
  
  TFileInfo info;
  if( type == KDEV_DIALOG){
    TDialogFileInfo dinfo;
    dinfo.rel_name = rel_name;
    dinfo.type = type;
    dinfo.dist = true;
    dinfo.install = false;
    //...
    dinfo.is_toplevel_dialog = true;
    
    new_subdir = m_prj->addDialogFileToProject(dinfo.rel_name,dinfo);
    
    
    info.rel_name = rel_name;
  }
  else{ // normal File
    info.rel_name = rel_name;
    info.type = type;
    info.dist = ( type != PO );
    
    info.install=false;
    info.install_location = "";
    new_subdir = m_prj->addFileToProject(rel_name,info);
    
    
  }
  m_prj->writeProject();
  m_prj->updateMakefilesAm();
  
  if(refresh)
    refreshTrees(&info);
  
  return new_subdir;
}

void CKDevelop::delFileFromProject(QString rel_filename){

  m_prj->removeFileFromProject(rel_filename);
  m_prj->writeProject();
  ComponentManager::self()->notifyRemovedFileFromProject(rel_filename);

  // ### Should be removed:
  refreshTrees();
}

bool CKDevelop::readProjectFile(QString file){
    QString str;
    QString extension;
    
    m_prj = new CProject(file);
    if(!(m_prj->readProject())){
	return false;
    }
    else {
	m_project=true;
    }
    
    
    
    extension=(m_prj->getProjectType()=="normal_c") ? "c" : "cpp";
    str = m_prj->getProjectDir() + m_prj->getSubDir() + m_prj->getProjectName().lower() + ".h";
    if(QFile::exists(str)){
	switchToFile(str);
    }
    str = m_prj->getProjectDir() + m_prj->getSubDir() + "main."+extension;
    if(QFile::exists(str)){
	switchToFile(str);
    }
    
    // set the menus enable
    // file menu
    
    enableCommand(ID_FILE_NEW);
    // doc menu
    enableCommand(ID_HELP_PROJECT_API);
    enableCommand(ID_HELP_USER_MANUAL);
    // build menu
    setToolMenuProcess(true);
    
    // prj menu
    enableCommand(ID_PROJECT_CLOSE);
    enableCommand(ID_PROJECT_ADD_FILE_EXIST);
    
    if(m_prj->isKDEProject() || m_prj->isQtProject()){
    }
    
    if (m_prj->isKDEProject()){
	enableCommand(ID_PROJECT_ADD_NEW_TRANSLATION_FILE);
    }
    else{
	disableCommand(ID_PROJECT_ADD_NEW_TRANSLATION_FILE);
    }
    if(m_prj->isCustomProject()){
	disableCommand(ID_PROJECT_FILE_PROPERTIES);
	enableCommand(ID_PROJECT_OPTIONS);
    }
    else{
	enableCommand(ID_PROJECT_FILE_PROPERTIES);
	enableCommand(ID_PROJECT_OPTIONS);
    }
    
    enableCommand(ID_PROJECT_REMOVE_FILE);
    enableCommand(ID_PROJECT_WORKSPACES);
    enableCommand(ID_BUILD_AUTOCONF);
    enableCommand(ID_PROJECT_MAKE_DISTRIBUTION);
    
    if (m_prj->getProjectType()!="normal_c")  // activate class wizard unless it is a normal C project
	{
	    enableCommand(ID_PROJECT_NEW_CLASS);
	    enableCommand(ID_CV_WIZARD);
	    enableCommand(ID_CV_GRAPHICAL_VIEW);
	    enableCommand(ID_CV_TOOLBAR_CLASS_CHOICE);
	    enableCommand(ID_CV_TOOLBAR_METHOD_CHOICE);
	}
    
    addRecentProject(file);
    return true;
}

void CKDevelop::newSubDir(){
  if(m_prj->getProjectType() == "normal_empty"){
    return; // no makefile handling
  }
  KMessageBox::information(0, i18n("You have added a new subdir to the project.\nWe will regenerate all Makefiles now."));
  setToolMenuProcess(false);
  slotStatusMsg(i18n("Running automake/autoconf and configure..."));
  showOutputView(true);
  m_messages_widget->prepareJob(m_prj->getProjectDir());
  (*m_messages_widget) << m_prj->getCompilationEnvironment();
  (*m_messages_widget) << m_make_cmd << " -f Makefile.dist  && ./configure" << m_prj->getConfigureArgs();

  m_messages_widget->startJob();
}

