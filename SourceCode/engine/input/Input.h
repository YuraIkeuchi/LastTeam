#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"
#include <Xinput.h>
#include<DirectXMath.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
using namespace Microsoft::WRL;



class Input
{
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	enum XBOX
	{
		UP = XINPUT_GAMEPAD_DPAD_UP,
		DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		B = XINPUT_GAMEPAD_B,
		A = XINPUT_GAMEPAD_A,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
		LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		LT,
		RT
	};

	//スティック
	enum STICK
	{
		L_UP,
		L_DOWN,
		L_LEFT,
		L_RIGHT,
		R_UP,
		R_DOWN,
		R_LEFT,
		R_RIGHT,
		XBOX_STICK_NUM
	};

	bool PushButton(XBOX Button);

	bool TriggerButton(XBOX Button);


	bool CheckTrigger(XBOX Button);
private:
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keypre[256] = {};


	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	WinApp* winapp = nullptr;
public:
	static Input* GetInstance();
	~Input();
	//namespace省略
	template <class T>
	using ComPtr = ComPtr<T>;
public:
	float rightAngle;

	struct MouseMove
	{
		LONG lX;
		LONG lY;
		LONG lZ;
	};

	//ゲームパッド（ボタン）
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LeftStick,
		Button_RightStick,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};

	//スティック
	enum StickKind
	{
		Up,
		Down,
		Right,
		Left
	};

	//初期化
	void Initialize(WinApp* winapp);
	//更新
	void Update();
	//キーの押下をチェック
	bool Pushkey(BYTE keyNumber);
	//キーのトリガー判定
	bool TriggerKey(BYTE keyNumber);

	void Finalize();

	/// <summary>
	/// キーの左ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseLeft();

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseMiddle();

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();

	void MouseMovePos(XMFLOAT2& MovePos);

public:
	//ゲームパッド
	bool LeftTriggerStick(int stick);
	bool RightTiltStick(int stick);
	bool RightTriggerStick(int stick);
	bool PushCrossKey(int CrossKey);
	bool TriggerCrossKey(int CrossKey);
public:
	const LONG& GetPositionX() { return gamePadState2.lX; }
	const LONG& GetPositionY() { return gamePadState2.lY; }
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }
	const float& GetMousePosX() { return MousePosX; }
	const float& GetMousePosY() { return MousePosY; }
	const float& GetRPosX() { return static_cast<float>(xinputState.Gamepad.sThumbLX); }
	const float& GetRPosY() { return static_cast<float>(xinputState.Gamepad.sThumbLY); }
	//プッシュ
	bool TiltStick(STICK Stick);
	bool TiltPushStick(STICK Stick, float DeadZone = 0.3f);
	const float& GetLeftControllerX() { return LeftControllerX; }
	const float& GetLeftControllerY() { return LeftControllerY; }
private:
	//ゲームパッドデバイス
	ComPtr<IDirectInputDevice8> devGamePad;
	DIJOYSTATE2 gamePadState = {};
	DIJOYSTATE2 oldGamePadState = {};
	DIJOYSTATE2 gamePadState2 = {};
	DIJOYSTATE2 oldGamePadState2 = {};
	XINPUT_STATE xinputState;
	XINPUT_STATE oldXinputState;

	bool is_push[32] = {};
	//スティックの無反応範囲
	LONG unresponsive_range = 200;
	LONG unresponsive_range2 = 200;
	float posX = 0;
	float posY = 0;
	float MousePosX = 0.0f;
	float MousePosY = 0.0f;
	float LeftControllerX = 0;
	float LeftControllerY = 0;

	struct StickPos
	{
		float x;
		float y;
	};

	bool StickInDeadZone(StickPos& Thumb);
};
