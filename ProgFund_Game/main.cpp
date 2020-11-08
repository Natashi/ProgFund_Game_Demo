#include "pch.h"

#include "source/Engine/ResourceManager.hpp"
#include "source/Engine/Input.hpp"
#include "source/Engine/Window.hpp"
#include "source/Engine/Scene.hpp"
#include "source/Engine/Object.hpp"

DWORD DxGetTime();

class Circle : public TaskBase {
public:
	Sprite2D sprite;
	double angle;

	Circle(Scene* parent, D3DXVECTOR2 position) : TaskBase(parent) {
		angle = 0;

		ResourceManager* resourceManager = ResourceManager::GetBase();
		auto textureCircle = resourceManager->LoadResource<TextureResource>("eff_magiccircle.png", "eff_magiccircle.png");

		sprite.SetTexture(textureCircle);
		sprite.SetSourceRectNormalized(DxRect<float>(0, 0, 1, 1));
		sprite.SetDestCenter();
		sprite.UpdateVertexBuffer();

		sprite.SetPosition(position);
	}

	virtual void Render() {
		sprite.SetAngleZ(angle);
		sprite.Render();
	}
	virtual void Update() {
		angle += 0.01;
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	HWND hWnd = nullptr;
	try {
		printf("Initializing application...\n");

		WindowMain* winMain = new WindowMain();
		winMain->Initialize(hInstance);
		hWnd = winMain->GetHandle();

		ResourceManager* resourceManager = new ResourceManager();
		resourceManager->Initialize();

		printf("Initialized application.\n");

		auto textureCircle = resourceManager->LoadResource<TextureResource>("eff_magiccircle.png", "eff_magiccircle.png");

		Scene* scene = new Scene();

		shared_ptr<Circle> circle1 = shared_ptr<Circle>(new Circle(scene, D3DXVECTOR2(320, 240)));
		scene->AddTask(circle1);

		shared_ptr<Circle> circle2 = shared_ptr<Circle>(new Circle(scene, D3DXVECTOR2(100, 140)));
		scene->AddTask(circle2);

		shared_ptr<Circle> circle3 = shared_ptr<Circle>(new Circle(scene, D3DXVECTOR2(420, 390)));
		scene->AddTask(circle3);

		{
			//Refresh rate -> 60fps
			double t_target_ms = 1000.0 / 60;

			DWORD current_time = DxGetTime();
			DWORD previous_time = current_time;

			DWORD accum_fps = 0;
			DWORD accum_update = 0;

			std::list<DWORD> listDelta;
			std::list<double> listFPS;

			MSG msg = { 0 };
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else {
					current_time = DxGetTime();
					DWORD delta_time = current_time - previous_time;

					accum_fps += delta_time;
					accum_update += delta_time;

					previous_time = current_time;

					if (accum_update > t_target_ms) {
						listDelta.push_back(accum_update);

						//Engine render
						winMain->BeginScene();
						{
							scene->Update();
							scene->Render();
						}
						winMain->EndScene();

						accum_update = 0;
					}

					//2 fps updates per second, hopefully
					if (accum_fps > 500) {
						DWORD sumMs = 0;
						for (DWORD& iMs : listDelta)
							sumMs += iMs;
						listDelta.clear();

						double fps = (t_target_ms * 1000) / (double)sumMs;
						listFPS.push_back(fps);
						if (listFPS.size() > 64)
							listFPS.pop_front();
						winMain->SetFPS(fps);

						//fpsCounter.setString(StringFormat("%.2f fps", fps));
						//printf("%.2f\n", fps);

						accum_fps = 0;
					}

					Sleep(1);
				}
			}
		}

		printf("Finalizing application...\n");

		ptr_release(resourceManager);
		ptr_release(winMain);

		printf("Finalized application.\n");
		return 0;
	}
	catch (std::exception& e) {
		MessageBoxA(hWnd, e.what(), "Unexpected Error", MB_ICONERROR | MB_APPLMODAL | MB_OK);
	}
	catch (EngineError& e) {
		MessageBoxA(hWnd, e.what(), "Engine Error", MB_ICONERROR | MB_APPLMODAL | MB_OK);
	}

	return 0;
}

DWORD DxGetTime() {
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nCounter;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nCounter);
	return (DWORD)(nCounter.QuadPart * 1000 / nFreq.QuadPart);
}