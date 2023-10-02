#include "Input.h"
#include <cmath>

#define STICK_MAX 32768.0f
#pragma comment (lib, "xinput.lib")
Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

bool Input::PushButton(XBOX Button)
{
	//トリガー
	if (Button == LT)
	{
		return xinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
			&& CheckTrigger(Button);
	}
	if (Button == RT)
	{
		return xinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
			&& CheckTrigger(Button);
	}
	return (xinputState.Gamepad.wButtons & Button)
		&& CheckTrigger(Button);
	////assert(0);
	return false;
}

bool Input::TriggerButton(XBOX Button)
{
	//トリガー
	if (Button == LT)
	{
		return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
			&& CheckTrigger(Button);
	}
	if (Button == RT)
	{
		return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
			&& CheckTrigger(Button);
	}
	return !(oldXinputState.Gamepad.wButtons & Button)
		&& CheckTrigger(Button);
	////assert(0);
	return false;
}

bool Input::CheckTrigger(XBOX Button)
{
	if (Button == LT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
	}
	if (Button == RT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
	}
	return xinputState.Gamepad.wButtons & Button;
	////assert(0);
	return false;
}

Input::~Input()
{
	//delete 	Input::GetIns();
	//	dinput.Reset();
	devkeyboard.Reset();
	devMouse.Reset();
}

void Input::Initialize(WinApp* winapp)
{
	HRESULT result;
	//winappのインスタンスを記録
	this->winapp = winapp;
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput = nullptr;
	result = DirectInput8Create(winapp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, &dinput, nullptr);
	//キーボードデバイ氏生成
	//ComPtr<IDirectInputDevice8>devkeyboard;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, nullptr);

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, nullptr);

	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(winapp->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	//mouse
	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(winapp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;
	{
		//前回のキー情報を保存
		memcpy(keypre, key, sizeof(key));
		//キーボード情報の取得開始
		result = devkeyboard->Acquire();
		//全キーの入力情報取得
		//BYTE key[256] = {};
		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}
	{
		// マウス
		result = devMouse->Acquire(); // マウス動作開始

		// 前回の入力を保存
		mouseStatePre = mouseState;

		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	//コントローラー
	oldXinputState = xinputState;
	ZeroMemory(&xinputState, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(0, &xinputState);
	if (dwResult == ERROR_SUCCESS)
	{
		LeftControllerX = static_cast<float>(xinputState.Gamepad.sThumbLX);
		LeftControllerY = static_cast<float>(xinputState.Gamepad.sThumbLY);

		//コントローラーが接続されている
	}
}

bool Input::Pushkey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key[keyNumber])
	{
		return true;
	}
	return false;
}

//トリガー
bool Input::TriggerKey(BYTE keyNumber)
{
	//前回の結果が０で今回の会話が１のとき
	if (!keypre[keyNumber] && key[keyNumber])
	{
		return true;
	}
	return false;
}

bool Input::PushMouseLeft()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[0])
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[2])
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouseLeft()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}

	// トリガーでない
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}


bool Input::TiltStick(STICK Stick)
{
	StickPos oldVec;
	StickPos vec;
	//右か左か
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick)
	{
		oldVec.x = oldXinputState.Gamepad.sThumbLX;
		oldVec.y = oldXinputState.Gamepad.sThumbLY;
		vec.x = xinputState.Gamepad.sThumbLX;
		vec.y = xinputState.Gamepad.sThumbLY;
	} else
	{
		oldVec.x = oldXinputState.Gamepad.sThumbRX;
		oldVec.y = oldXinputState.Gamepad.sThumbRY;
		vec.x = xinputState.Gamepad.sThumbRX;
		vec.y = xinputState.Gamepad.sThumbRY;
	}
	if (!StickInDeadZone(oldVec))
	{
		return false;
	}
	if (StickInDeadZone(vec))
	{
		return false;
	}

	bool result = false;

	if (Stick % 4 == L_UP)
	{
		result = 0.3f < (vec.y / STICK_MAX);
	} else if (Stick % 4 == L_DOWN)
	{
		result = vec.y / STICK_MAX < -0.3f;
	} else if (Stick % 4 == L_RIGHT)
	{
		result = 0.3f < (vec.x / STICK_MAX);
	} else if (Stick % 4 == L_LEFT)
	{
		result = vec.x / STICK_MAX < -0.3f;
	}

	return result;
}

bool Input::LeftTriggerStick(int stick)
{
	//左
	if (xinputState.Gamepad.sThumbLX < -unresponsive_range && !(oldXinputState.Gamepad.sThumbLX < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//右
	if (xinputState.Gamepad.sThumbLX > unresponsive_range && !(xinputState.Gamepad.sThumbLX > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//後ろ
	if (xinputState.Gamepad.sThumbLY> unresponsive_range && !(xinputState.Gamepad.sThumbLY> unresponsive_range) && stick == Down)
	{
		return true;
	}
	//前
	if (xinputState.Gamepad.sThumbLY< -unresponsive_range && !(xinputState.Gamepad.sThumbLY< -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}

bool Input::RightTiltStick(int stick)
{
	unresponsive_range = 200;

	//左
	if (gamePadState.lRx < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//右
	if (gamePadState.lRx > unresponsive_range && stick == Right)
	{
		return true;
	}
	//後ろ
	if (gamePadState.lRy > unresponsive_range && stick == Down)
	{
		return true;
	}
	//前
	if (gamePadState.lRy < -unresponsive_range && stick == Up)
	{
		return true;
	}

	rightAngle = static_cast<float>(atan2(0 - gamePadState.lRx, 0 - gamePadState.lRy));

	posX = static_cast<float>(gamePadState.lRx);
	posY = static_cast<float>(gamePadState.lRy);
	return false;
}


void Input::MouseMovePos(XMFLOAT2& MovePos)
{
	POINT mousePosition;

	GetCursorPos(&mousePosition);

	HWND hwnd = winapp->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);

	MovePos.x = (float)mousePosition.x;
	MovePos.y = (float)mousePosition.y;
}

bool Input::RightTriggerStick(int stick)
{
	//左
	if (gamePadState.lRx < -unresponsive_range && !(oldGamePadState.lRx < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//右
	if (gamePadState.lRx > unresponsive_range && !(oldGamePadState.lRx > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//後ろ
	if (gamePadState.lRy > unresponsive_range && !(oldGamePadState.lRy > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//前
	if (gamePadState.lRy < -unresponsive_range && !(oldGamePadState.lRy < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}

//
//bool Input::PushButton(int Button)
//{
//	for (int i = 0; i < 32; i++)
//	{
//		if (!(gamePadState.rgbButtons[i] & 0x80))
//		{
//			continue;
//		}
//
//		switch (i)
//		{
//		case 0:
//			is_push[A] = true;
//			break;
//		case 1:
//			is_push[B] = true;
//			break;
//		case 2:
//			is_push[X] = true;
//			break;
//		case 3:
//			is_push[Y] = true;
//			break;
//		case 4:
//			is_push[LB] = true;
//			break;
//		case 5:
//			is_push[RB] = true;
//			break;
//		case 6:
//			is_push[BACK] = true;
//			break;
//		case 7:
//			is_push[START] = true;
//			break;
//		case 8:
//			is_push[Button_LeftStick] = true;
//			break;
//		case 9:
//			is_push[Button_RightStick] = true;
//			break;
//
//		default:
//			break;
//		}
//	}
//
//	for (int i = 0; i < Cross_Up; i++)
//	{
//		if (is_push[i] == true)
//		{
//			if (is_push[i] == is_push[Button])
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}

bool Input::PushCrossKey(int CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[Cross_Up] = true;
			break;
		case 4500:
			is_push[Cross_Up] = true;
			is_push[Cross_Right] = true;
			break;
		case 9000:
			is_push[Cross_Right] = true;
			break;
		case 13500:
			is_push[Cross_Right] = true;
			is_push[Cross_Down] = true;
			break;
		case 18000:
			is_push[Cross_Down] = true;
			break;
		case 22500:
			is_push[Cross_Down] = true;
			is_push[Cross_Left] = true;
			break;
		case 27000:
			is_push[Cross_Left] = true;
			break;
		case 31500:
			is_push[Cross_Left] = true;
			is_push[Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Input::TriggerCrossKey(int CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF && oldGamePadState.rgdwPOV[0] == 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[Cross_Up] = true;
			break;
		case 4500:
			is_push[Cross_Up] = true;
			is_push[Cross_Right] = true;
			break;
		case 9000:
			is_push[Cross_Right] = true;
			break;
		case 13500:
			is_push[Cross_Right] = true;
			is_push[Cross_Down] = true;
			break;
		case 18000:
			is_push[Cross_Down] = true;
			break;
		case 22500:
			is_push[Cross_Down] = true;
			is_push[Cross_Left] = true;
			break;
		case 27000:
			is_push[Cross_Left] = true;
			break;
		case 31500:
			is_push[Cross_Left] = true;
			is_push[Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Input::TiltPushStick(STICK Stick, float DeadZone)
{
	StickPos vec;
	StickPos oldVec;

	//右か左か
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick)
	{
		oldVec.x = oldXinputState.Gamepad.sThumbLX;
		oldVec.y = oldXinputState.Gamepad.sThumbLY;

		vec.x = xinputState.Gamepad.sThumbLX;
		vec.y = xinputState.Gamepad.sThumbLY;
	} else
	{
		oldVec.x = oldXinputState.Gamepad.sThumbRX;
		oldVec.y = oldXinputState.Gamepad.sThumbRY;

		vec.x = xinputState.Gamepad.sThumbRX;
		vec.y = xinputState.Gamepad.sThumbRY;
	}

	//if (!StickInDeadZone(oldVec))return false;
	if (StickInDeadZone(vec))
	{
		return false;
	}

	bool result = false;
	if (Stick % 4 == L_UP)
	{
		result = DeadZone < (vec.y / STICK_MAX);
	} else if (Stick % 4 == L_DOWN)
	{
		result = vec.y / STICK_MAX < -DeadZone;
	} else if (Stick % 4 == L_RIGHT)
	{
		result = DeadZone < (vec.x / STICK_MAX);
	} else if (Stick % 4 == L_LEFT)
	{
		result = vec.x / STICK_MAX < -DeadZone;
	}

	return result;
}

bool Input::StickInDeadZone(StickPos& Thumb)
{
	bool x = false, y = false;
	if ((Thumb.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f
		&& Thumb.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f))
	{
		Thumb.x = 0.0f;
		x = true;
	}
	if ((Thumb.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f
		&& Thumb.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f))
	{
		Thumb.y = 0.0f;
		y = true;
	}
	if (x && y)
	{
		return true;
	}
	return false;
}

void Input::Finalize()
{

}
