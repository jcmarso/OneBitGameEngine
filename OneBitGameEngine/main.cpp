#include "onebitge.h"

int main() {
	OneBitGE Instance;
	Instance.Init(512, 288);
	Instance.GetTileSheetFromFile("EditedTileset_32x32_1bit.dat");
	Instance.GetSpriteSheetFromFile("player32x32.dat");
	Instance.GetMapFromFile("map46x27.dat");
	Instance.AssignCodeToTiles(32, 32, 320, 416);
	Instance.AssignCodeToSprites(32, 32, 96, 192);
	Instance.LoadMapToBuffer();
	Instance.LoadSpriteToBuffer(32, 32, 0);

	// Game loop
	while (true) {
		//Instance.Input();
		Instance.Animation();
		Instance.Draw();
	}
	return 0;
}