#include "PoolTest.h"

void Create_Pool_Test_Environment(_PoolTest_Group& obj_PoolTest_Group)
{
	char szTempPath[MAX_BUFF_50]   = {'\0'};

	//���������ļ���
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s", obj_PoolTest_Group.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//����JsonĿ¼
	sprintf_safe(szTempPath, MAX_BUFF_50, "%s/Test", obj_PoolTest_Group.m_sz_ProcName);
#ifdef WIN32
	_mkdir(szTempPath);
#else
	mkdir(szTempPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif
}

bool Create_Class_Pool_Test_Cpp(_PoolTest_Group& obj_PoolTest_Group)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	//��������H�ļ���
	sprintf_safe(szPathFile, 200, "%s/Test/Test_Pool_Test.cpp", 
		obj_PoolTest_Group.m_sz_ProcName);

	FILE* pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return false;
	}

	sprintf_safe(szTemp, 200, "#include \"DB_Op.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//���Ҫ�õ���.h�ļ�ͷ
	for(int i = 0; i < (int)obj_PoolTest_Group.m_vec_PoolTest_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", 
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//���ɶ�Ӧ���Է���
	for(int i = 0; i < (int)obj_PoolTest_Group.m_vec_PoolTest_Info.size(); i++)
	{
		//���Keyֵ
		char szKeyValue[MAX_BUFF_50] = {'\0'};
		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_n_IsKey == 1)
			{
				sprintf_safe(szKeyValue, MAX_BUFF_50, "%s", obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				break;
			}
		}

		if(strlen(szKeyValue) == 0)
		{
			//���û�����������������²��Դ���
			sprintf_safe(szTemp, 200, "//%s_Pool is not set key,can not make test code.\n",
				obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			continue;
		}

		//���ɲ��Լ����������
		sprintf_safe(szTemp, 200, "void check_%s_pool_insert()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tint nBefCount =  %s_Pool::getInstance()->get_used_count();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\t%s* p_%s =  %s_Pool::getInstance()->get_free_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tif(NULL == p_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get Free is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tint nEndCount =  %s_Pool::getInstance()->get_used_count();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(nEndCount - nBefCount != 1)\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get Count is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//��ʼ��ֵ
		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(strcmp(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\tp_%s->set_%s((char*)(\"%s\"));\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tp_%s->set_%s(%s);\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//���µõ��������ԣ����бȽ�
		sprintf_safe(szTemp, 200, "\t%s* p_pool_%s =  %s_Pool::getInstance()->get_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(NULL == p_pool_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get set used is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(strcmp(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\tif(strcmp(p_pool_%s->get_%s(), \"%s\") != 0)\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Attr %s is not equal.\\n\");\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tif(p_pool_%s->get_%s() != %s)\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Attr %s is not equal.\\n\");\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_insert]Check insert is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//����ɾ�����Ժ���
		sprintf_safe(szTemp, 200, "void check_%s_pool_delete()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s_Pool::getInstance()->del_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s* p_del_%s =  %s_Pool::getInstance()->get_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(NULL != p_del_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_delete]delete is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_delete]Check delete is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//�������ݿ����
		sprintf_safe(szTemp, 200, "void check_%s_pool_insert_record()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\t%s object;\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//��ʼ��ֵ
		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(strcmp(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\tobject.set_%s((char*)(\"%s\"));\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tobject.set_%s(%s);\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\tinsert_%s(object);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Table_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_insert_record]Check insert_record is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//�������ݿ����
		sprintf_safe(szTemp, 200, "void check_%s_pool_db_load()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\t%s_Pool::getInstance()->load_data();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\t%s* p_pool_%s =  %s_Pool::getInstance()->get_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(NULL == p_pool_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get set used is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(strcmp(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\tif(strcmp(p_pool_%s->get_%s(), \"%s\") != 0)\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Attr %s is not equal.\\n\");\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tif(p_pool_%s->get_%s() != %s)\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Attr %s is not equal.\\n\");\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_db_load]Check db_load is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "void check_%s_pool_db_save()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_db_save]Check db_save is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//�������ݿ����
		sprintf_safe(szTemp, 200, "void check_%s_pool_delete_record()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\t%s object;\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//��ʼ��ֵ
		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(strcmp(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\tobject.set_%s((char*)(\"%s\"));\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tobject.set_%s(%s);\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\tdelete_%s(object);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Table_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_delete_record]Check delete_record is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//�����ܲ�����ں���
		sprintf_safe(szTemp, 200, "void check_%s()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_insert();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_delete();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_insert_record();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_db_load();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_db_save();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "\t//check_%s_pool_delete_record();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	//����main����
	sprintf_safe(szTemp, 200, "int main(int argc, char* argv[])\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tConnPool::GetInstance()->InitConnPool(\"localhost\",\"mysql\",\"block2\",100,20);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_PoolTest_Group.m_vec_PoolTest_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\tcheck_%s();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\treturn 0;\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	fclose(pFile);

	return true;
}

bool Create_Class_Pool_Test_Makefile(_XML_Proc& obj_XML_Proc, _PoolTest_Group& obj_PoolTest_Group)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	//�Զ�����makefile.define�ļ�
	sprintf_safe(szPathFile, 200, "%s/Test/Makefile.define", obj_XML_Proc.m_sz_ProcName);

	FILE* pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return false;
	}

	sprintf_safe(szTemp, 200, "# *****************************\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# predefine\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# *****************************\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "CC = g++\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "AR = ar\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "CFLAGS = -g -O2 -D__LINUX__\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "#set Lua lib path\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "INCLUDES = -I/usr/local/mysql/include -I../DataWrapper -I../DBWrapper -I../MysqlCommon -I../ShareMemory -I../rapidjson\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "LIBS = -L/usr/lib64 -L/usr/lib -L/usr/local/lib64 -L/usr/local/mysql/lib -lmysqlclient\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# *****************************\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# rule\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# *****************************\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# Here are some rules for converting .cpp -> .o\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, ".SUFFIXES: .cpp .o\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, ".cpp.o:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t@$(CC) -fPIC $(CFLAGS) ${INCLUDES} -c -g $*.cpp\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t@echo '----- '$*.cpp' is compiled ok! -----'\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "# Here are some rules for converting .c -> .o\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, ".SUFFIXES: .c .o\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, ".c.o:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t@$(CC) $(CFLAGS) -c $*.c \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t@echo '----- '$*.c' is compiled ok! -----'\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);

	sprintf_safe(szPathFile, 200, "%s/Test/Makefile", obj_XML_Proc.m_sz_ProcName);

	pFile = fopen(szPathFile, "wb");
	if(NULL == pFile)
	{
		return false;
	}

	sprintf_safe(szTemp, 200, "include Makefile.define\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "PATS = ");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\t../ShareMemory/ShareMemory.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\t../MysqlCommon/conn_pool.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\t../MysqlCommon/mysql_encap.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\t../DBWrapper/DB_Op.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	
	for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\t../DataWrapper/%s.o \\\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	
	sprintf_safe(szTemp, 200, "\t./Test_Pool_Test.o\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "OBJS = ");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tShareMemory.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tconn_pool.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tmysql_encap.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tDB_Op.o \\\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\t%s.o \\\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\tTest_Pool_Test.o\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "APP_NAME = Test_Pool_Test\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	
	sprintf_safe(szTemp, 200, "$(APP_NAME):$(PATS)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\t$(CC) -rdynamic -o $(APP_NAME) $(OBJS) $(LIBS)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\trm -rf *.o\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "clean:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\trm -rf *.o $(APP_NAME)\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "cl:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\trm -rf *.o\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	fclose(pFile);

	return true;
}

bool CreatePool_Test(_XML_Proc& obj_XML_Proc, _PoolTest_Group& obj_PoolTest_Group)
{
	Create_Pool_Test_Environment(obj_PoolTest_Group);

	Create_Class_Pool_Test_Cpp(obj_PoolTest_Group);

	Create_Class_Pool_Test_Makefile(obj_XML_Proc, obj_PoolTest_Group);

	return true;
}