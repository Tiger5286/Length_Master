#include "SceneMain.h"
#include "DxLib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Game.h"

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
}

void SceneMain::Update()
{
	// キー入力を取得
	for (int i = 0; i < 256; i++)
	{
		m_prevKeyState[i] = m_nowKeyState[i];
	}
	GetHitKeyStateAll(m_nowKeyState);
	// モードを変更
	if (m_nowKeyState[KEY_INPUT_1])
	{
		m_selectObj = Object::Start;
	}
	else if (m_nowKeyState[KEY_INPUT_2])
	{
		m_selectObj = Object::Clear;
	}
	else if (m_nowKeyState[KEY_INPUT_3])
	{
		m_selectObj = Object::Obstacles;
	}
	else if (m_nowKeyState[KEY_INPUT_4])
	{
		m_selectObj = Object::Delete;
	}
	// マウス位置を取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	Vector2 mousePos = Vector2(mouseX,mouseY);
	// マウス入力を取得
	m_prevMouseInput = m_nowMouseInput;
	m_nowMouseInput = GetMouseInput();
	bool isTriggerClick = m_nowMouseInput == MOUSE_INPUT_1 && m_prevMouseInput != MOUSE_INPUT_1;
	bool isPressClick = m_nowMouseInput == MOUSE_INPUT_1;
	bool isReleasedClick = m_nowMouseInput != MOUSE_INPUT_1 && m_prevMouseInput == MOUSE_INPUT_1;
	// クリックしたときの処理
	// スタート、ゴール
	if (isTriggerClick)
	{
		switch (m_selectObj)
		{
		case Object::Start:
			m_startPos = mousePos;
			break;
		case Object::Clear:
			m_clearPos = mousePos;
			break;
		}
	}
	// 障害物
	if (m_selectObj == Object::Obstacles)
	{
		if (isTriggerClick)
		{
			newRect.x = mousePos.x;
			newRect.y = mousePos.y;
		}
		if (isPressClick)
		{
			newRect.w = mousePos.x - newRect.x;
			newRect.h = mousePos.y - newRect.y;
		}
		if (isReleasedClick)
		{
			if (newRect.w < 0)
			{
				newRect.x += newRect.w;
				newRect.w = -newRect.w;
			}
			if (newRect.h < 0)
			{
				newRect.y += newRect.h;
				newRect.h = -newRect.h;
			}
			m_obstacles.push_back(newRect);
			newRect.w = 0.0f;
			newRect.h = 0.0f;
		}
	}
	// 幅、高さが0以下の障害物は削除する
	for (auto& rect : m_obstacles)
	{
		if (rect.w < 1 || rect.h < 1)
		{
			printfDx("削除対象の障害物を発見しました\n");
		}
	}
	std::erase_if(m_obstacles, [&](const Rect& rect)
		{
			return rect.w < 1 || rect.h < 1;
		});
	// 削除
	if (m_selectObj == Object::Delete)
	{
		if (isTriggerClick)
		{
			std::erase_if(m_obstacles, [&](const Rect& rect)
				{
					return rect.CheckPointInRect(mousePos);
				});
		}
	}

	if (m_nowKeyState[KEY_INPUT_S] && !m_prevKeyState[KEY_INPUT_S])
	{
		WriteBinary();
	}
	if (m_nowKeyState[KEY_INPUT_Q] && !m_prevKeyState[KEY_INPUT_Q])
	{
		WriteCSV();
	}

	if (m_nowKeyState[KEY_INPUT_R] && !m_prevKeyState[KEY_INPUT_R])
	{
		m_startPos = Vector2::Zero();
		m_clearPos = Vector2::Zero();
		m_obstacles.clear();
		clsDx();
	}

	if (m_nowKeyState[KEY_INPUT_T] && !m_prevKeyState[KEY_INPUT_T])
	{
		char path[128] = {};
		int result = KeyInputSingleCharString(100, 100, 128, path, true);
		if (result == 1)
		{
			LoadBinary(path);
		}
		else
		{
			printfDx("入力が正しく終了しませんでした\n");
		}
	}
	if (m_nowKeyState[KEY_INPUT_G] && !m_prevKeyState[KEY_INPUT_G])
	{
		char path[128] = {};
		int result = KeyInputSingleCharString(100, 100, 128, path, true);
		if (result == 1)
		{
			LoadCSV(path);
		}
		else
		{
			printfDx("入力が正しく終了しませんでした\n");
		}
	}

	if (m_nowKeyState[KEY_INPUT_Y] && !m_prevKeyState[KEY_INPUT_Y])
	{
		char str[128] = {};
		int result = KeyInputSingleCharString(100, 100, 128, str, true);
		if (result == 1)
		{
			m_goalLength = std::stof(str);
		}
		else
		{
			printfDx("入力が正しく終了しませんでした\n");
		}
	}
}

void SceneMain::Draw()
{
	switch (m_selectObj)
	{
	case Object::Start:
		DrawCircle(20, 50, 10, 0x00ffff);
		break;
	case Object::Clear:
		DrawCircle(20, 50, 10, 0xffff00);
		break;
	case Object::Obstacles:
		DrawBox(10, 40, 30, 60, 0xff0000, true);
		break;
	case Object::Delete:
		DrawLine(20, 40, 40, 60, 0xff0000, 5);
		DrawLine(20, 60, 40, 40, 0xff0000, 5);
		break;
	}

	DrawCircle(m_startPos.x, m_startPos.y, 10, 0x00ffff);
	DrawCircle(m_clearPos.x, m_clearPos.y, 10, 0xffff00);

	for (const auto& rect : m_obstacles)
	{
		rect.Draw(0x222222, true);
	}

	newRect.Draw(0x222222, true);

	DrawBox(0, Game::kScreenHeight - 100, Game::kScreenWidth, Game::kScreenHeight, 0x222222, true);

	DrawString(0,0,"Sでバイナリ出力　QでCSV出力　Rでリセット　Tでバイナリをロード　GでCSVをロード　Yでゴール距離を設定",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "Length:%.2f", m_goalLength);
}

void SceneMain::WriteBinary()
{
	char path[128] = {};
	int result = KeyInputSingleCharString(100, 100, 128, path, true);
	if (result != 1)
	{
		printfDx("入力が正しく終了しませんでした\n");
		return;
	}

	std::ofstream ofs(path, std::ios::out | std::ios::binary);

	if (!ofs)
	{
		printfDx("ファイルオープンに失敗しました\n");
		return;
	}

	ofs.write(reinterpret_cast<const char*>(&m_startPos.x), sizeof(float));
	ofs.write(reinterpret_cast<const char*>(&m_startPos.y), sizeof(float));

	ofs.write(reinterpret_cast<const char*>(&m_clearPos.x), sizeof(float));
	ofs.write(reinterpret_cast<const char*>(&m_clearPos.y), sizeof(float));
	ofs.write(reinterpret_cast<const char*>(&m_goalLength), sizeof(float));

	for (const auto& rect : m_obstacles)
	{
		ofs.write(reinterpret_cast<const char*>(&rect.x), sizeof(float));
		ofs.write(reinterpret_cast<const char*>(&rect.y), sizeof(float));
		ofs.write(reinterpret_cast<const char*>(&rect.w), sizeof(float));
		ofs.write(reinterpret_cast<const char*>(&rect.h), sizeof(float));
	}

	ofs.close();

	printfDx("出力しました\n");
}

void SceneMain::LoadBinary(char* path)
{
	int handle = FileRead_open(path);

	if (handle == -1)
	{
		printfDx("ファイルが開けませんでした\n");
		return;
	}

	m_startPos = Vector2::Zero();
	m_clearPos = Vector2::Zero();
	m_obstacles.clear();
	clsDx();

	FileRead_read(&m_startPos.x, sizeof(float), handle);
	FileRead_read(&m_startPos.y, sizeof(float), handle);

	FileRead_read(&m_clearPos.x, sizeof(float), handle);
	FileRead_read(&m_clearPos.y, sizeof(float), handle);
	FileRead_read(&m_goalLength, sizeof(float), handle);

	while (!FileRead_eof(handle))
	{
		Rect rect;
		FileRead_read(&rect.x, sizeof(float), handle);
		FileRead_read(&rect.y, sizeof(float), handle);
		FileRead_read(&rect.w, sizeof(float), handle);
		FileRead_read(&rect.h, sizeof(float), handle);
		m_obstacles.push_back(rect);
	}

	FileRead_close(handle);
}

void SceneMain::WriteCSV()
{
	char path[128] = {};
	int result = KeyInputSingleCharString(100, 100, 128, path, true);
	if (result != 1)
	{
		printfDx("入力が正しく終了しませんでした\n");
		return;
	}

	std::ofstream ofs(path);
	if (!ofs) return;

	std::vector<float> datas;
	datas.reserve(5 + m_obstacles.size() * 4);
	datas.push_back(m_startPos.x);
	datas.push_back(m_startPos.y);
	datas.push_back(m_clearPos.x);
	datas.push_back(m_clearPos.y);
	datas.push_back(m_goalLength);
	for (const auto& rect : m_obstacles)
	{
		datas.push_back(rect.x);
		datas.push_back(rect.y);
		datas.push_back(rect.w);
		datas.push_back(rect.h);
	}
	for (const auto& data : datas)
	{
		ofs << data << ",";
	}
}

void SceneMain::LoadCSV(char* path)
{
	std::vector<float> datas;
	std::ifstream ifs(path);

	std::string line;
	if (std::getline(ifs, line))
	{
		std::stringstream ss(line);
		std::string cell;
		while (std::getline(ss, cell, ','))
		{
			if (!cell.empty())
			{
				datas.push_back(std::stof(cell));
			}
		}
	}

	m_startPos = Vector2(datas[0], datas[1]);
	m_clearPos = Vector2(datas[2], datas[3]);
	m_goalLength = datas[4];
	
	m_obstacles.clear();
	int rectNum = (datas.size() - 5) / 4;
	for (int i = 0; i < rectNum; i++)
	{
		Rect rect;
		rect.x = datas[(5 + i * 4) + 0];
		rect.y = datas[(5 + i * 4) + 1];
		rect.w = datas[(5 + i * 4) + 2];
		rect.h = datas[(5 + i * 4) + 3];
		m_obstacles.push_back(rect);
	}
}
