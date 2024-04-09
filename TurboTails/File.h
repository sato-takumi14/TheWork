#pragma once

#include"Main.h"
#include<fstream>

using namespace cereal;

class File
{
public:

/**************************************************************************************************/
	/*バイナリデータ*/
	template <typename DATA>
	static void WriteDataBinary(const DATA& data, crstring name) {

		fstream file;
		file.open(name.c_str(), ios::out | ios::binary);

		file.write((char*)&data, sizeof(data));

		file.close();

	}

	template <typename DATA>
	static const DATA LoadDataBinary(crstring name) {

		fstream file;
		file.open(name.c_str(), ios::in | ios::binary);

		DATA data{};

		file.read((char*)&data, sizeof(data));

		file.close();

		return data;

	}

	template <typename DATA>
	static void WriteDatasBinary(const list<DATA>& datas, crstring name) {

		fstream file;
		file.open(name.c_str(), ios::out | ios::binary);

		for (auto& data : datas) {

			file.write((char*)&data, sizeof(data));

		}

		file.close();

	}

	template <typename DATA>
	static const list<DATA> LoadDatasBinary(crstring name) {

		fstream file;
		file.open(name.c_str(), ios::in | ios::binary);

		list<DATA> datas{};

		while (true) {

			DATA data{};
			file.read((char*)&data, sizeof(data));

			if (file.eof())break;

			datas.emplace_back(data);

		}

		file.close();

		return datas;

	}

/**************************************************************************************************/
	/*文字データ*/
	template <typename DATA>
	static void WriteData(const DATA& data, crstring name) {

		fstream file;
		file.open(name.c_str(), ios::out);

		file.write((char*)&data, sizeof(data));

		file.close();

	}

	template <typename DATA>
	static const DATA LoadData(crstring name) {

		fstream file;
		file.open(name.c_str(), ios::in);

		DATA data{};
		file.read((char*)&data, sizeof(data));

		file.close();

		return data;

	}

	template <typename DATA>
	static void WriteDatas(const list<DATA> datas, crstring name) {

		fstream file;
		file.open(name.c_str(), ios::out);

		for (auto& data : datas) {

			file.write((char*)&data, sizeof(data));

		}

		file.close();

	}

	template <typename DATA>
	static const list<DATA> LoadDatas(crstring name) {

		fstream file;
		file.open(name.c_str(), ios::in);

		list<DATA> datas{};

		while (true) {

			DATA data{};
			file.read((char*)&data, sizeof(data));

			if (file.eof())break;

			datas.emplace_back(data);

		}

		file.close();

		return datas;

	}

/**************************************************************************************************/
	/*JSONファイル*/
	template <class DATA>
	static void JsonSave(const DATA& data, crstring path) {
		stringstream stream;
		{
			cereal::JSONOutputArchive json(stream);
			json(data);
		}
		ofstream file(path, ios::out);
		file << stream.str();
		file.close();
		stream.clear();
	}

	template <class DATA>
	static void JsonSave(const DATA& data, crstring path, crstring tag) {
		stringstream stream;
		{
			cereal::JSONOutputArchive json(stream);
			json(cereal::make_nvp(tag, data));
		}
		ofstream file(path, ios::out);
		file << stream.str();
		file.close();
		stream.clear();
	}

	template <class DATA>
	static const DATA JsonLoad(crstring path) {
		stringstream stream;
		ifstream file(path, ios::in);
		stream << file.rdbuf();
		DATA data;
		cereal::JSONInputArchive json(stream);
		json(data);
		file.close();
		return data;
	}

	template <class DATA>
	static void JsonSaves(const list<DATA>& datas, crstring path) {
		stringstream stream;
		{
			cereal::JSONOutputArchive json(stream);
			json(datas);
		}
		ofstream file(path, ios::out);
		file << stream.str();
		file.close();
		stream.clear();
	}

	template <class DATA>
	static const list<DATA> JsonLoads(crstring path) {
		list<DATA> datas;
		stringstream stream;
		ifstream file(path, ios::in);
		stream << file.rdbuf();
		cereal::JSONInputArchive json(stream);
		json(datas);
		file.close();
		return datas;
	}

	template <class DATA>
	static const list<DATA> JsonLoadsTag(crstring path, crstring tag) {
		stringstream stream;
		ifstream file(path, ios::in);
		stream << file.rdbuf();
		cereal::JSONInputArchive json(stream);
		list<DATA> datas;
		json(make_nvp(tag, datas));
		file.close();
		return datas;
	}

	template <class DATA>
	static const DATA JsonLoadTag(crstring path, crstring tag) {
		stringstream stream;
		ifstream file(path, ios::in);
		stream << file.rdbuf();
		cereal::JSONInputArchive json(stream);
		DATA data;
		json(make_nvp(tag, data));
		file.close();
		return data;
	}

	template <class DATA>
	static void JsonSavesApp(const DATA& data, crstring path, crstring tag) {
		stringstream stream;
		{
			cereal::JSONOutputArchive json(stream);
			json(make_nvp(tag, data));
		}
		ofstream file(path, ios::out | ios::app);
		file << stream.str();
		file.close();
		file.clear();
	}

	template <class DATA>
	static void JsonSaves(const unordered_map<string, list<DATA>>& datas, crstring path) {
		stringstream stream;
		{
			cereal::JSONOutputArchive json(stream);
			for (auto& data : datas) {
				json(cereal::make_nvp(data.first, data.second));
			}
		}
		ofstream file(path, ios::out);
		file << stream.str();
		file.close();
		file.clear();
	}

};
