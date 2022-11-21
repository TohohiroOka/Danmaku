#include "Title.h"
#include "SceneManager.h"
#include "Boss1.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include <imgui.h>
#include "DebugText.h"
#include "WindowApp.h"

using namespace DirectX;

void Title::Initialize()
{
	Sprite::LoadTexture("title", "Resources/Sprite/title.png", true);
	sp = Sprite::Create("title");
	sp->SetPosition({ 0,0 });
	sp->SetTexSize({ float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) });
	sp->SetSize({ 1280, 720 });
	sp->Update();

}

void Title::Update()
{
	DirectInput* input = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();

	//シーンの移行
	if (input->TriggerKey(DIK_RETURN)|| xinput->PushButton(XInputManager::PUD_BUTTON::PAD_A))
	{
		Boss1* nextScene = new Boss1();
		SceneManager::SetNextScene(nextScene);
	}

	DebugText* text = DebugText::GetInstance();
	text->Print("push enter or pad A", 100, 500, { 0.0f,0.9f,0.0f }, 5.0f);
	text = nullptr;
}

void Title::DrawNotPostB()
{
}

void Title::Draw()
{
}

void Title::DrawNotPostA()
{
	Sprite::PreDraw(cmdList);
	sp->Draw();
	DebugText::GetInstance()->DrawAll();
	Sprite::PostDraw();
}

void Title::Finalize()
{
}

void Title::ImguiDraw()
{
	//float baseColor[3];//ベースカラ―
	//float metalness;//金属度(0 or 1)
	//float specular;//鏡面反射度
	//float roughness;//粗さ

	//baseColor[0] = circle->GetBaseColor().x;
	//baseColor[1] = circle->GetBaseColor().y;
	//baseColor[2] = circle->GetBaseColor().z;
	//metalness = circle->GetMetalness();
	//specular = circle->GetSpecular();
	//roughness = circle->GetRoughness();

	////ライトon/off
	//static bool lightAct1 = false;
	//static bool lightAct2 = false;
	//static bool lightAct3 = false;

	//ImGui::Begin("Material");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(300, 200));
	//ImGui::ColorEdit3("baseColor", baseColor, ImGuiColorEditFlags_Float);
	//ImGui::SliderFloat("metalness", &metalness, 0, 1);
	//ImGui::SliderFloat("specular", &specular, 0, 1);
	//ImGui::SliderFloat("roughness", &roughness, 0, 1);
	//ImGui::Checkbox("Light1", &lightAct1);
	//ImGui::Checkbox("Light2", &lightAct2);
	//ImGui::Checkbox("Light3", &lightAct3);
	//ImGui::End();

	//circle->SetBaseColor({ baseColor[0],baseColor[1],baseColor[2] });
	//circle->SetMetalness(metalness);
	//circle->SetSpecular(specular);
	//circle->SetRoughness(roughness);

	//light->SetDirLightActive(0, lightAct1);
	//light->SetDirLightActive(1, lightAct2);
	//light->SetDirLightActive(2, lightAct3);
}

void Title::GetConstbufferNum()
{
}
