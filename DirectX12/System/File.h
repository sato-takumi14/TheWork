#pragma once

#define  WIN32_LEAN_AND_MEAN
//#define  _WIN32_WINNT _WIN32_WINNT_WIN7

#include"System\HelperFunction.h"

#pragma warning (disable:4192)

/*Excelを操作するためのタイプライブラリを読み込む*/
//#import "C:\Program Files\Microsoft Office\root\vfs\ProgramFilesCommonX86\Microsoft Shared\Office16\MSO.DLL" no_auto_exclude auto_rename dual_interfaces
//#import "C:\Program Files\Microsoft Office\root\vfs\ProgramFilesCommonX86\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" rename("CommandBars", "ExcelCommandBars")
//#import "C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE" rename("RGB", "Excel2010RGB") rename("DialogBox", "ExcelDialogBox") rename("CopyFile", "ExcelCopyFile") rename("ReplaceText", "ExcelReplaceText6")

//Microsoft Office Object Library
#import "libid:2DF8D04C-5BFA-101B-BDE5-00AA0044DE52" no_auto_exclude auto_rename dual_interfaces
//Microsoft Visual Basic for Applications Extensibillity
#import "libid:0002E157-0000-0000-C000-000000000046" dual_interfaces
//Mcrosoft Excel Object Library
#import "libid:00020813-0000-0000-C000-000000000046" no_auto_exclude auto_search auto_rename dual_interfaces

namespace File
{

	/*絶対パス*/
	constexpr auto AbsolutePath = "C:\\Users\\user\\Desktop\\Git\\private\\プログラム\\DirextX12\\";

	/*バイナリ*/
	namespace Binary
	{

		/*テキストファイル*/
		class Text
		{
		public:

			/*出力*/
			template <class DATA>
			inline static void Write(const DATA& data, crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::out | std::ios::binary);
				file.write(const_cast<const char*>(&data), sizeof(data));
				file.close();
			}

			template <class DATA>
			inline static void Write(const std::list<DATA>& datas, crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::out | std::ios::binary);
				for (auto& data : datas) { file.write(const_cast<const char*>(&data), sizeof(data)); }
				file.close();
			}

			/*読み込み*/
			template <class DATA>
			inline static const DATA Load(crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::in | std::ios::binary);
				DATA data;
				file.read(const_cast<const char*>(&data), sizeof(data));
				file.close();
				return data;
			}

			template <class DATA>
			inline static const std::list<DATA> Load(crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::in | std::ios::binary);
				std::list<DATA> datas{};
				while (true) {
					DATA data{};
					file.read(const_cast<const char*>(&data), sizeof(data));
					if (file.eof())break;
					datas.push_back(data);
				}
				file.close();
				return datas;
			}


		};

		/*JSONファイル*/
		class Json
		{
		public:

			/*出力*/
			template <class DATA>
			inline static void Write(const DATA& data, crstring name) {
				std::stringstream stream;
				{
					cereal::JSONOutputArchive json(stream);
					json(data);
				}
				std::ofstream file(name, std::ios::out | std::ios::binary);
				file << stream.str();
				file.close();
				stream.clear();
			}

			template <class DATA>
			inline static void Write(const DATA& data, crstring name, crstring tag) {
				std::stringstream stream;
				{
					cereal::JSONOutputArchive json(stream);
					json(cereal::make_nvp(tag, data));
				}
				std::ofstream file(name, std::ios::out | std::ios::binary);
				file << stream.str();
				file.close();
				stream.clear();
			}

			/*読み込み*/
			template <class DATA>
			inline static const DATA Load(crstring name) {
				std::stringstream stream;
				std::ifstream file(name, std::ios::in | std::ios::binary);
				stream << file.rdbuf();
				cereal::JSONInputArchive json(stream);
				DATA data;
				json(data);
				file.close;
				return data;
			}

			template <class DATA>
			inline static const DATA Load(crstring name, crstring tag) {
				std::stringstream stream;
				std::ifstream file(name, std::ios::in | std::ios::binary);
				stream << file.rdbuf();
				cereal::JSONInputArchive json(stream);
				DATA data;
				json(cereal::make_nvp(tag, data));
				file.close;
				return data;
			}

		};

	}

	/*文字データ*/
	namespace Letter
	{

		/*テキストファイル*/
		class Text
		{
		public:

			/*出力*/
			template <class DATA>
			inline static void Write(const DATA& data, crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::out);
				file.write(const_cast<const char*>(data), sizeof(data));
				file.close();
			}

			template <class DATA>
			inline static void Write(const std::list<DATA>& datas, crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::out);
				for (auto& data : datas) { file.write(const_cast<const char*>(&data), sizeof(data)); }
				file.close();
			}

			/*読み込み*/
			template <class DATA>
			inline static const DATA Load(crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::in);
				DATA data;
				file.read(const_cast<const char*>(&data), sizeof(data));
				file.close();
				return data;
			}

			template <class DATA>
			inline static const std::list<DATA> Load(crstring name) {
				std::fstream file;
				file.open(name.c_str(), std::ios::in);
				std::list<DATA> datas;
				while (true) {
					DATA data{};
					file.read(const_cast<const char*>(&data), sizeof(data));
					if (file.eof())break;
					datas.push_back(data);
				}
				file.close();
				return datas;
			}

		};
		
		/*JSONファイル*/
		class Json
		{
		public:

			/*出力*/
			template <class DATA>
			inline static void Write(const DATA& data, crstring name) {
				std::stringstream stream;
				{
					cereal::JSONOutputArchive json(stream);
					json(data);
				}
				std::ofstream file(name, std::ios::out);
				file << stream.str();
				file.close();
				stream.clear();
			}

			template <class DATA>
			inline static void Write(const DATA& data, crstring name, crstring tag) {
				std::stringstream stream;
				{
					cereal::JSONOutputArchive json(stream);
					json(cereal::make_nvp(tag, data));
				}
				std::ofstream file(name, std::ios::out);
				file << stream.str();
				file.close();
				stream.clear();
			}

			/*読み込み*/
			template <class DATA>
			inline static const DATA Load(crstring name) {
				std::stringstream stream;
				std::ifstream file(name, std::ios::in);
				stream << file.rdbuf();
				cereal::JSONInputArchive json(stream);
				DATA data;
				json(data);
				file.close;
				return data;
			}

			template <class DATA>
			inline static const DATA Load(crstring name, crstring tag) {
				std::stringstream stream;
				std::ifstream file(name, std::ios::in);
				stream << file.rdbuf();
				cereal::JSONInputArchive json(stream);
				DATA data;
				json(cereal::make_nvp(tag, data));
				file.close;
				return data;
			}

		};

	}

	class ExcelDevice
	{
	public:

		inline static void MasterInit() { assert(CoInitialize(nullptr) == S_OK); }
		inline static void MasterUninit() { CoUninitialize(); }
		
		template <class DATA>
		inline static const std::vector<DATA> Road(crstring name, cruint page = 1) {

			Excel::_ApplicationPtr app;
			std::vector<DATA> data;

			if (SUCCEEDED(app.CreateInstance(L"Excel.Application"))) {

				/*警告が出ないように*/
				app->DisplayAlerts[0] = VARIANT_FALSE;

				try {

					/*ワークブックを開く*/
					Excel::WorkbooksPtr workbooks = app->Workbooks;
					/*絶対パスで指定しないとエラー*/
					string file = (AbsolutePath + name);
					Excel::_WorkbookPtr workbook = workbooks->Open(file.c_str());

					/*アクティブ・シートを取得*/
					Excel::SheetsPtr worksheets = workbook->Worksheets;
					Excel::_WorksheetPtr worksheet = worksheets->Item[page];

					/*セル範囲からデータを読み込む*/
					/*ワークシート内で使用されている範囲を取得*/
					Excel::RangePtr range = worksheet->UsedRange;

					/*セル範囲からデータを取得*/
					_variant_t arr = range->Value2;

					SAFEARRAY* psa;
					if (arr.vt & VT_BYREF)psa = (*arr.pparray);
					else psa = arr.parray;

					long rows, cols;
					/*行数を取得*/
					SafeArrayGetUBound(psa, 1, &rows);
					/*列数を取得*/
					SafeArrayGetUBound(psa, 2, &cols);

					/*VARIANTのポインタ*/
					_variant_t* p = nullptr;
					if (SUCCEEDED(SafeArrayAccessData(psa, (void**)(&p)))) {

						for (int i = 0; i < rows; i++) {
							for (int j = 0; j < cols; j++) {
								_variant_t var = *(p + j * rows + i);
								if (var.vt == VT_R8) data.push_back(static_cast<DATA>(var));								
							}
						}

						SafeArrayUnaccessData(psa);

					}

					worksheet.Release();
					worksheets.Release();
					workbook->Close();
					workbook.Release();
					workbooks.Release();

				}
				catch (_com_error& e) {

				}

				app.Release();

			}

			return data;

		}

		template <class DATA>
		inline static const std::vector<std::vector<DATA>> Road(crstring path, crstring page = "") {

			Excel::_ApplicationPtr app;
			std::vector<DATA> data;

			if (SUCCEEDED(app.CreateInstance(L"Excel.Application"))) {

				/*警告が出ないように*/
				app->DisplayAlerts[0] = VARIANT_FALSE;

				try {

					/*ワークブックを開く*/
					Excel::WorkbooksPtr workbooks = app->Workbooks;
					/*絶対パスで指定しないとエラー*/
					string file = (AbsolutePath + path);
					Excel::_WorkbookPtr workbook = workbooks->Open(file.c_str());

					/*アクティブ・シートを取得*/
					Excel::SheetsPtr worksheets = workbook->Worksheets;
					Excel::_WorksheetPtr worksheet = worksheets->Item[page.c_str()];

					/*セル範囲からデータを読み込む*/
					/*ワークシート内で使用されている範囲を取得*/
					Excel::RangePtr range = worksheet->UsedRange;

					/*セル範囲からデータを取得*/
					_variant_t arr = range->Value2;

					SAFEARRAY* psa;
					if (arr.vt & VT_BYREF)psa = (*arr.pparray);
					else psa = arr.parray;

					long rows, cols;
					/*行数を取得*/
					SafeArrayGetUBound(psa, 1, &rows);
					/*列数を取得*/
					SafeArrayGetUBound(psa, 2, &cols);

					/*VARIANTのポインタ*/
					_variant_t* p = nullptr;
					if (SUCCEEDED(SafeArrayAccessData(psa, (void**)(&p)))) {

						for (int i = 0; i < rows; i++) {
							for (int j = 0; j < cols; j++) {
								_variant_t var = *(p + j * rows + i);
								if (var.vt == VT_R8) data.push_back(static_cast<DATA>(var));
							}
						}

						SafeArrayUnaccessData(psa);

					}

					worksheet.Release();
					worksheets.Release();
					workbook->Close();
					workbook.Release();
					workbooks.Release();

				}
				catch (_com_error& e) {

				}

				app.Release();

			}

			return data;

		}

	};

}