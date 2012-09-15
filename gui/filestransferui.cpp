#include "filestransferui.h"
#include "share/AppRegData.h"
#include <QSettings>
#include <QFileDialog>

FilesTransferUI::FilesTransferUI(QObject *parent) :
    QObject(parent)
{
}

void FilesTransferUI::download(FilesManager* manager)
{
    if(SDriveEngine::inst()->downloadMngr)
    {
        if(SDriveEngine::inst()->downloadMngr->getState() == NetworkManager::EBusy) return;
    }

    QSettings settings(COMPANY_NAME, APP_NAME);

    QList<ItemInfo::Data> item = manager->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
    int index = SDriveEngine::inst()->filesUI->getCurrFileItemId(manager);

    qDebug() << "FilesTransferUI::download item[index].downloadLink:" << item[index].downloadLink;
    qDebug() << "FilesTransferUI::download item[index].self:" << item[index].self;

    QString downloadLink(item[index].downloadLink);

    if(!downloadLink.isEmpty())
    {
        if(SDriveEngine::inst()->checkUI->slotCheckWorkDir(false))
        {
            QString fileName = settings.value(WORK_DIR).toString() + "/" + item[index].name;
            QString fileType =  item[index].fileType;

            SDriveEngine::inst()->downloadMngr.reset(new DownloadFileManager(SDriveEngine::inst()->parent));
            SDriveEngine::inst()->downloadMngr->startDownload(QUrl(downloadLink), fileName, fileType);
        }
        else CommonTools::msg(SET_DIR_REMINDER_MSG);
    }
}

void FilesTransferUI::upload(void)
{
    if(SDriveEngine::inst()->elStates[EAFoldersViewFocused])
    {
        CommonTools::msg("To upload a file please select a folder in \"Drive\" view");
        return;
    }

    if(SDriveEngine::inst()->uploadFileMngr)
    {
        if(SDriveEngine::inst()->uploadFileMngr->getState() == NetworkManager::EBusy) return;
    }

    QString fileName = QFileDialog::getOpenFileName(SDriveEngine::inst()->parent, "Uploading file", QDir::homePath(), "All files(*)");

    qDebug() << "FilesTransferUI::upload fileName:" << fileName;

    if(!fileName.isEmpty())
    {
        //        ItemInfo item = *SDriveEngine::inst()->foldersMngr->getParser()->getXMLHandler()->getItemInfo();
        //        int index = SDriveEngine::inst()->foldersUI->getCurrFolderItemId();

        QList<ItemInfo::Data> item = SDriveEngine::inst()->getCurrFilesMngr()->getParser()->getXMLHandler()->getItemInfo()->getFileItems();
        int index = SDriveEngine::inst()->filesUI->getCurrFileItemId(SDriveEngine::inst()->getCurrFilesMngr());

        qDebug() << "FilesTransferUI::upload index:" << index;

        QString uploadLink(item[index].uploadLink + "/?convert=false");

        qDebug() << "FilesTransferUI::upload uploadLink:" << uploadLink;

        if(!uploadLink.isEmpty())
        {
            SDriveEngine::inst()->uploadFileMngr.reset(new UploadFileManager(SDriveEngine::inst()->parent));
            connect(SDriveEngine::inst()->uploadFileMngr.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));
            SDriveEngine::inst()->uploadFileMngr->startUpload(uploadLink, fileName);
        }
    }
}

void FilesTransferUI::slotDownload(void)
{
    if(SDriveEngine::inst()->elStates[EAFoldersViewFocused]) download(SDriveEngine::inst()->aFoldersMngr.data());
    else download(SDriveEngine::inst()->getCurrFilesMngr());
}

void FilesTransferUI::slotUpload(void)
{
    upload();
}