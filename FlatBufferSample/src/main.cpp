#include "monster_generated.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"

using namespace MyGame::Sample;

#include <iostream>
#include <fstream>

using namespace std;
using namespace flatbuffers;

// flatbuffers/util.h ‚ÅéŒ¾‚³‚ê‚Ä‚¢‚éŠÖ”‚ÌÀ‘•
bool MySaveFile(const char* name, const char* buf, size_t len, bool binary) {
	ofstream ofs(name, binary ? ofstream::binary : ofstream::out);
	if (!ofs.is_open()) return false;
	ofs.write(buf, len);
	return !ofs.bad();
}

// flatbuffers/util.h ‚ÅéŒ¾‚³‚ê‚Ä‚¢‚éŠÖ”‚ÌÀ‘•
bool MyLoadFile(const char* name, bool binary, std::string* buf) {
	std::ifstream ifs(name, binary ? std::ifstream::binary : std::ifstream::in);
	if (!ifs.is_open()) {
		return false;
	}
	if (binary) {
		// The fastest way to read a file into a string.
		ifs.seekg(0, std::ios::end);
		auto size = ifs.tellg();
		(*buf).resize(static_cast<size_t>(size));
		ifs.seekg(0, std::ios::beg);
		ifs.read(&(*buf)[0], (*buf).size());
	}
	else {
		// This is slower, but works correctly on all platforms for text files.
		std::ostringstream oss;
		oss << ifs.rdbuf();
		*buf = oss.str();
	}
	return !ifs.bad();
}


int main() {
	flatbuffers::FlatBufferBuilder builder(1024);

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

	builder.Finish(orc);

	/* •Ê‚Ì‘‚«•û
	MonsterBuilder monster_builder(builder);
	monster_builder.add_pos(&position);
	monster_builder.add_hp(hp);
	monster_builder.add_name(name);
	monster_builder.add_inventory(inventory);
	monster_builder.add_color(Color_Red);
	monster_builder.add_weapons(weapons);
	monster_builder.add_equipped_type(Equipment_Weapon);
	monster_builder.add_equipped(axe.Union());
	auto orc = monster_builder.Finish();
	*/

	uint8_t* monster = builder.GetBufferPointer();
	int size = builder.GetSize();

	auto ok = MySaveFile("monsterdata.bin",
		reinterpret_cast<const char*>(
			builder.GetBufferPointer()),
		builder.GetSize(),
		true);

	// bool LoadFile(const char* name, bool binary, std::string* buf)
	string loadData = "";
	auto loadOk = MyLoadFile("monsterdata.bin", true, &loadData);
	auto monster_2 = GetMonster(loadData.c_str());

	std::cout << "hp : " << monster_2->hp() << std::endl;            // `80`
	std::cout << "mana : " << monster_2->mana() << std::endl;        // default value of `150`
	std::cout << "name : " << monster_2->name()->c_str() << std::endl;        // "MyMonster"
}