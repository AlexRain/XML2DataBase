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

	//���Ҫ�õ���.h�ļ�ͷ
	for(int i = 0; i < (int)obj_PoolTest_Group.m_vec_PoolTest_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#include \"DataWrapper/%s.h\"\n", 
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
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
				obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			continue;
		}

		//���ɲ��Լ����������
		sprintf_safe(szTemp, 200, "void check_%s_pool_insert()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tint nBefCount =  %s_Pool::getInstance()->get_used_count();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s* p_%s =  %s_Pool::getInstance()->get_free_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(NULL == p_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get Free is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tint nEndCount =  %s_Pool::getInstance()->get_used_count();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(nEndCount - nBefCount != 1)\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get Count is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
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
				sprintf_safe(szTemp, 200, "\tp_%s->set_%s(\"%s\")\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tp_%s->set_%s(%s)\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//���µõ��������ԣ����бȽ�
		sprintf_safe(szTemp, 200, "\t%s* p_pool_%s =  %s_Pool::getInstance()->get_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(NULL == p_pool_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Get set used is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		for(int j = 0; j < (int)obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column.size(); j++)
		{
			if(strcmp(obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Type, "char") == 0)
			{
				sprintf_safe(szTemp, 200, "\tif(strcmp(p_pool_%s->get_%s(), \"%s\") != 0\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Attr %s is not equal.\\n\");\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
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
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Value);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_insert]Attr %s is not equal.\\n\");\n",
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
					obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_vec_PoolTest_Column[j].m_sz_Column_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_insert]Check insert is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//����ɾ�����Ժ���
		sprintf_safe(szTemp, 200, "void check_%s_pool_delete()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s_Pool::getInstance()->del_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s* p_del_%s =  %s_Pool::getInstance()->get_used_object(%s);\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName,
			szKeyValue);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tif(NULL != p_del_%s)\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\tprintf(\"[check_%s_pool_delete]delete is Error.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t\treturn;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_delete]Check delete is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//�������ݿ����
		sprintf_safe(szTemp, 200, "void check_%s_pool_db_load()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_db_load]Check is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		sprintf_safe(szTemp, 200, "void check_%s_pool_db_save()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tprintf(\"[check_%s_pool_db_save]Check is pass.\\n\");\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//�����ܲ�����ں���
		sprintf_safe(szTemp, 200, "void check_%s()\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_insert();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_delete();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_db_load();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tcheck_%s_pool_db_save();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	//����main����
	sprintf_safe(szTemp, 200, "int main(int argc, char* argv[])\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_PoolTest_Group.m_vec_PoolTest_Info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\tcheck_%s();\n",
			obj_PoolTest_Group.m_vec_PoolTest_Info[i].m_sz_ClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\treturn 0;\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n"); 
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	fclose(pFile);

	return true;
}

bool CreatePool_Test(_PoolTest_Group& obj_PoolTest_Group)
{
	Create_Pool_Test_Environment(obj_PoolTest_Group);

	Create_Class_Pool_Test_Cpp(obj_PoolTest_Group);

	return true;
}