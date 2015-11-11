#include "XmlOpeation.h"

CXmlOpeation::CXmlOpeation(void)
{
	m_pTiXmlDocument = NULL;
	m_pRootElement   = NULL;
}

CXmlOpeation::~CXmlOpeation(void)
{
	Close();
}

bool CXmlOpeation::Init(const char* pFileName)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	//��ø�Ԫ��
	m_pRootElement = m_pTiXmlDocument->RootElement();

	return true;
}

bool CXmlOpeation::Init_String(const char* pXMLText)
{
	//�����ַ���
	Close();
	m_pTiXmlDocument = new TiXmlDocument();
	m_pTiXmlDocument->Parse(pXMLText, 0, TIXML_DEFAULT_ENCODING); 

	//��ø�Ԫ��
	m_pRootElement = m_pTiXmlDocument->RootElement();

	return true;
}

void CXmlOpeation::Close()
{
	if(NULL != m_pTiXmlDocument)
	{
		delete m_pTiXmlDocument;
		m_pTiXmlDocument = NULL;
		m_pRootElement   = NULL;
	}
}

char* CXmlOpeation::GetData(const char* pName, const char* pAttrName)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);
	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->Attribute(pAttrName);
	}

	return NULL;
}

char* CXmlOpeation::GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = NULL;
	if(NULL == pNextTiXmlElement)
	{
		pTiXmlElement = m_pRootElement->FirstChildElement(pName);
		pNextTiXmlElement = pTiXmlElement;
	}
	else
	{
		pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
		pNextTiXmlElement = pTiXmlElement;
	}

	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->Attribute(pAttrName);
	}

	return NULL;
}

char* CXmlOpeation::GetData_Text(const char* pName)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);
	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->GetText();
	}

	return NULL;
}

char* CXmlOpeation::GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = NULL;
	if(NULL == pNextTiXmlElement)
	{
		pTiXmlElement = m_pRootElement->FirstChildElement(pName);
		pNextTiXmlElement = pTiXmlElement;
	}
	else
	{
		pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
		pNextTiXmlElement = pTiXmlElement;
	}

	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->GetText();
	}

	return NULL;
}

bool CXmlOpeation::Parse_XML_File(char* pFileName, _Proc_Info& obj_Proc_Info)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	TiXmlNode* pMainNode = NULL;
	//��ø�Ԫ��
	m_pRootElement = m_pTiXmlDocument->RootElement();
	sprintf_safe(obj_Proc_Info.m_szProcName, MAX_BUFF_50, "%s", m_pRootElement->ToElement()->Attribute("ProcName"));


	//ѭ����ӡ��ÿһ������
	if(NULL == m_pRootElement)
	{
		return false;
	}

	//��ø�Ԫ�ص���Ԫ��
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode=pMainNode->NextSiblingElement())
	{
		_Class_Info obj_Class_Info;

		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();

		//��ø�Ԫ�ص�����
		sprintf_safe(obj_Class_Info.m_szXMLName, MAX_BUFF_50, pMainElement->Value());
		sprintf_safe(obj_Class_Info.m_szDesc, MAX_BUFF_100, "%s", pMainElement->Attribute("desc"));
		char* pPoolCount = (char* )pMainElement->Attribute("PoolCount");
		if(NULL != pPoolCount)
		{
			obj_Class_Info.m_nPollCount =atoi(pPoolCount);
		}

		//printf("Root=%s.\n", m_pRootElement->Value());
		//����������
		TiXmlNode* pNode = NULL;

		for(pNode = pMainElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
		{
			int nType = pNode->Type();
			if(nType == TiXmlText::TINYXML_ELEMENT)
			{
				//printf("Name=%s,Values=%s.\n", pNode->Value(), pNode->ToElement()->GetText());
				_Property objProperty;
				sprintf_safe(objProperty.m_szPropertyName, MAX_BUFF_50, "%s", pNode->Value());
				sprintf_safe(objProperty.m_szProperyType, MAX_BUFF_50, "%s", pNode->ToElement()->GetText());

				//ͬʱ����ַ�����󳤶�
				char* pLength = (char* )pNode->ToElement()->Attribute("length");
				if(NULL != pLength)
				{
					objProperty.m_nLength = (int)atoi(pLength);
				}
				
				sprintf_safe(objProperty.m_szDesc, 100, "%s", pNode->ToElement()->Attribute("desc"));

				obj_Class_Info.m_vecProperty.push_back(objProperty);
			}
		}

		obj_Proc_Info.obj_vec_Class_Info.push_back(obj_Class_Info);
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;

	return true;
}

bool CXmlOpeation::Parse_XML_define_File(char* pFileName, _Proc_Define_Info& obj_Proc_Define_Info)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		return false;
	}

	TiXmlNode* pMainNode     = NULL;

	//��ø�Ԫ��
	m_pRootElement = m_pTiXmlDocument->RootElement();

	if(NULL == m_pRootElement)
	{
		return false;
	}

	//��ù�������
	sprintf_safe(obj_Proc_Define_Info.m_szProcName, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("ProcName"));

	//ѭ����ȡԤ������Ϣ
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode = pMainNode->NextSiblingElement())
	{
		_Define_Info obj_Define_Info;

		//���Lua�ļ���Ϣ
		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();
		sprintf_safe(obj_Define_Info.m_szSrcType, MAX_BUFF_50, "%s", pMainElement->Attribute("srcType"));
		sprintf_safe(obj_Define_Info.m_szTagType, MAX_BUFF_50, "%s", pMainElement->Attribute("tagType"));
		sprintf_safe(obj_Define_Info.m_szDesc, MAX_BUFF_100, "%s", pMainElement->Attribute("desc"));
		obj_Proc_Define_Info.obj_vec_Define_Info.push_back(obj_Define_Info);
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;

	return true;
}
