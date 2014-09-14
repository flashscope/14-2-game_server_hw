#pragma once

#define CLIENT_CONFIG			"ClientConfig.xml"

#define PATH_MODEL		L"ModelFiles\\"
#define PATH_TEXTURE	L"ModelFiles\\"

#define MODEL_UNIT_SWORD		PATH_MODEL L"UnitSword.x"
#define MODEL_UNIT_PIKE			PATH_MODEL L"UnitPike.x"
#define MODEL_UNIT_ARROW		PATH_MODEL L"UnitArrow.x"
#define MODEL_UNIT_KNIGHT		PATH_MODEL L"UnitKnight.x"
#define MODEL_UNIT_GUARD		PATH_MODEL L"UnitGuard.x"
#define MODEL_UNIT_KING			PATH_MODEL L"UnitKing.x"

#define MODEL_CORPS_DESTROY_A		PATH_MODEL L"Corps_Destroy_A.x"
#define MODEL_CORPS_DESTROY_B		PATH_MODEL L"Corps_Destroy_B.x"
#define MODEL_CORPS_DEFENSE_A		PATH_MODEL L"Corps_Defense_A.x"
#define MODEL_CORPS_DEFENSE_B		PATH_MODEL L"Corps_Defense_B.x"
#define MODEL_CORPS_RUSH_A			PATH_MODEL L"Corps_Rush_A.x"
#define MODEL_CORPS_RUSH_B			PATH_MODEL L"Corps_Rush_B.x"


#define PATH_MAP		L"MapFiles\\"

// #define HEIGHT_MAP				PATH_MAP L"heightmap_128_128.bmp"
#define HEIGHT_MAP				PATH_MAP L"heightmap_129_129.bmp"
#define HEIGHT_TEX				PATH_MAP L"heightmap_texture_1024_1024_hills.jpg"

// #define HEIGHT_MAP				PATH_MAP L"heightmap_129_129.bmp"
// #define HEIGHT_TEX				PATH_MAP L"map_tex.bmp"

// #define HEIGHT_MAP				PATH_MAP L"heightmap_1024_1024.bmp"
// #define HEIGHT_TEX				PATH_MAP L"heightmap_texture.tga"

// #define HEIGHT_MAP				PATH_MAP L"heightmap_1024_1024_korea.bmp"
// #define HEIGHT_TEX				PATH_MAP L"heightmap_texture_1024_1024_korea.bmp"

// #define HEIGHT_TEX				PATH_MAP L"heightmap_128_128.bmp"
// #define HEIGHT_TEX				PATH_MAP L"heightmap_tex_128_128.bmp"


#define PATH_SPRITE		L"SpriteFiles\\"

#define SPRITE_SKYBOX_BACK					PATH_SPRITE L"SkyBox_Back.png"
#define SPRITE_SKYBOX_FRONT					PATH_SPRITE L"SkyBox_Front.png"
#define SPRITE_SKYBOX_LEFT					PATH_SPRITE L"SkyBox_Left.png"
#define SPRITE_SKYBOX_RIGHT					PATH_SPRITE L"SkyBox_Right.png"
#define SPRITE_SKYBOX_TOP					PATH_SPRITE L"SkyBox_Top.png"
#define SPRITE_SKYBOX_BOTTOM				PATH_SPRITE L"SkyBox_Bottom.png"

#define SPRITE_CURSOR_DEFAULT				PATH_SPRITE L"cursor_default.png"
#define SPRITE_CURSOR_ATTACK				PATH_SPRITE L"cursor_attack.png"
#define SPRITE_CURSOR_CLICK					PATH_SPRITE L"cursor_clicked.png"
#define SPRITE_CURSOR_OVER_CORPS			PATH_SPRITE L"cursor_corps_clickable.png"
#define SPRITE_CURSOR_UNRECHEABLE			PATH_SPRITE L"cursor_unreachable.png"
#define SPRITE_CURSOR_CAMERA_ROTATING		PATH_SPRITE L"cursor_camera_rotating.png"
#define SPRITE_CURSOR_CORPS_MOVABLE			PATH_SPRITE L"cursor_movable.png"
#define SPRITE_CURSOR_CORPS_MOVABLE_CLICK	PATH_SPRITE L"cursor_movable_clicked.png"
#define SPRITE_CURSOR_OVER_PICKED_CORPS		PATH_SPRITE L"cursor_over_clicked_corps.png"

#define SPRITE_UI_MINIMAP					PATH_SPRITE L"UI_Map.png"
#define SPRITE_UI_REGENBAR					PATH_SPRITE L"UI_RegenBar.png"
#define SPRITE_UI_REGENFLAG					PATH_SPRITE L"UI_RegenFlag.png"
#define SPRITE_UI_CORPS_MARK_BLUE			PATH_SPRITE L"UI_CorpsMark_Blue.png"
#define SPRITE_UI_CORPS_MARK_RED			PATH_SPRITE L"UI_CorpsMark_Red.png"
#define SPRITE_UI_CORPS_FLAG_BLUE			PATH_SPRITE L"UI_CorpsMark_Flag_Blue.png"
#define SPRITE_UI_CORPS_FLAG_RED			PATH_SPRITE L"UI_CorpsMark_Flag_Red.png"
#define SPRITE_UI_CORPS_FLAG_GRAY			PATH_SPRITE L"UI_CorpsMark_Flag_Gray.png"

#define SPRITE_IMAGE_LOADING				PATH_SPRITE L"Loading.png"
#define SPRITE_IMAGE_RESULT_WIN				PATH_SPRITE L"ResultWin.png"
#define SPRITE_IMAGE_RESULT_LOSE			PATH_SPRITE L"ResultLose.png"

#define SPRITE_CORPS_TYPE_ARROW				PATH_SPRITE L"corps_type_arrow.png"
#define SPRITE_CORPS_TYPE_GUARD				PATH_SPRITE L"corps_type_guard.png"
#define SPRITE_CORPS_TYPE_KING				PATH_SPRITE L"corps_type_king.png"
#define SPRITE_CORPS_TYPE_KNIGHT			PATH_SPRITE L"corps_type_knight.png"
#define SPRITE_CORPS_TYPE_PIKE				PATH_SPRITE L"corps_type_pike.png"
#define SPRITE_CORPS_TYPE_SWORD				PATH_SPRITE L"corps_type_sword.png"

#define PATH_SOUND			L"SoundFiles\\"

#define SOUND_FILE_LOOP						PATH_SOUND L"loop.bank"
#define SOUND_FILE_MASTER_BANK				PATH_SOUND L"Master Bank.bank"
#define SOUND_FILE_MASTER_BANK_STRINGS		PATH_SOUND L"Master Bank.strings.bank"
#define SOUND_FILE_SFX_BANK					PATH_SOUND L"sfx.bank"


#define PATH_SHADER		L"ShaderFiles\\"

#define SELECT_SHADER			PATH_SHADER L"SelectShader.fx"
#define FIGHT_SHADER			PATH_SHADER L"FightShader.fx"
#define MAP_SHADER				PATH_SHADER L"MapShader.fx"