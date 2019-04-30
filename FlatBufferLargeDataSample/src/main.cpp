#include "monster_generated.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"

using namespace LargeData::Sample;

#include <iostream>
#include <fstream>

#include "TimeMeasure/TimeMeasure.h"
#include "FileIo/FlatBufferFileIo.h"

using namespace std;

int main() {

	// データ作成
	TimeMeasure timeCreateData;

	timeCreateData.start();

	flatbuffers::FlatBufferBuilder builder(30000);

	vector<flatbuffers::Offset<Monster>> monsters;

	for (int i = 0; i < 200000; ++i) {
		auto weapon_one_name = builder.CreateString("Sword");
		short weapon_one_damage = 3;

		auto weapon_two_name = builder.CreateString("Axe");
		short weapon_two_damage = 5;

		auto sword = CreateWeapon(builder, weapon_one_name, weapon_one_damage);
		auto axe = CreateWeapon(builder, weapon_two_name, weapon_two_damage);

		auto name = builder.CreateString("Orc");
		unsigned char treasure[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto inventory = builder.CreateVector(treasure, 10);

		vector<flatbuffers::Offset<Weapon>> weapons_vector;
		weapons_vector.push_back(sword);
		weapons_vector.push_back(axe);
		auto weapons = builder.CreateVector(weapons_vector);

		Vec3 points[] = { Vec3(1.0f, 2.0f, 3.0f), Vec3(4.0f, 5.0f, 6.0f) };
		auto path = builder.CreateVectorOfStructs(points, 2);

		auto position = Vec3(1.0f, 2.0f, 3.0f);

		int hp = 300;
		int mana = 150;

		auto orc = CreateMonster(builder, &position, mana, hp, name, inventory,
			Color_Red, weapons, Equipment_Weapon, axe.Union(), path);

		monsters.push_back(orc);
	}

	auto rootData = CreateRootData(builder, builder.CreateVector(monsters));

	builder.Finish(rootData);
	string createDataTimeString = timeCreateData.end();

	// ファイル出力
	TimeMeasure outputFileTime;
	outputFileTime.start();
	uint8_t* monster = builder.GetBufferPointer();
	int size = builder.GetSize();

	FlatBufferFileIo fileIo;
	auto ok = fileIo.saveFile("monsterdata.bin",
		reinterpret_cast<const char*>(
			builder.GetBufferPointer()),
		builder.GetSize(),
		true);
	string outputFileTimeString = outputFileTime.end();

	// ファイル入力

	TimeMeasure inputFileTime;
	inputFileTime.start();

	// bool LoadFile(const char* name, bool binary, std::string* buf)
	string loadData = "";
	auto loadOk = fileIo.loadFile("monsterdata.bin", true, &loadData);
	auto monster_2 = GetRootData(loadData.c_str());

	string inputFileTimeString = inputFileTime.end();

	cout << "CreateTime: " << createDataTimeString.c_str() << endl;
	cout << "OutputFileTime: " << outputFileTimeString.c_str() << endl;
	cout << "InputFileTime: " << inputFileTimeString.c_str() << endl;
}