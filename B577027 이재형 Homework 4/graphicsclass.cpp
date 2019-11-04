////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;

	m_Text = 0;

	Objs = 0;
	Polys = 0;

	CamPos.x = 0.0f;
	CamPos.y = 100.0f;
	CamPos.z = -200.0f;

	CamRot.x = 30.0f;
	CamRot.y = 0.0f;
	CamRot.z = 0.0f;

	PreX = 0.0f;
	PreY = 0.0f;

	PlayerModel.model = 0;
	PlayerModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ComModel.model = 0;
	ComModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BallModel.model = 0;
	BallModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}
GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(CamPos.x, CamPos.y, CamPos.z);
	m_Camera->SetRotation(CamRot.x, CamRot.y, CamRot.z);

	//  스카이박스 생성
	m_Skybox = new SkyboxClass;
	result = m_Skybox->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not Initialize Skybox.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	Models.push_back(GroundModel);
	Models.push_back(PlayerModel);
	Models.push_back(ComModel);
	Models.push_back(BallModel);

	//  obj파일들을 불러옵니다.
	ObjParser* parser = new ObjParser;

	GroundModel.filename = "../Engine/data/objs/ground.obj";
	PlayerModel.filename = "../Engine/data/objs/Player.obj";
	ComModel.filename = "../Engine/data/objs/Player.obj";
	BallModel.filename = "../Engine/data/objs/Ball.obj";

	GroundModel.texturename = L"../Engine/data/textures/metal.dds";
	PlayerModel.texturename = L"../Engine/data/textures/Player.dds";
	ComModel.texturename = L"../Engine/data/textures/Com.dds";
	BallModel.texturename = L"../Engine/data/textures/M33.dds";

	for (int i = 0; i < Models.size() - 1; i++)
	{
		parser->Parse(Models.at(i).filename);
		Models.at(i).model->Initialize(m_D3D->GetDevice(), "../B577027 이재형 Homework 4/model.txt", Models.at(i).texturename);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		Objs++;
		Polys += (parser->vertexCount) / 3;
	}

	delete parser;

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);	// specular power: lower value = greater effect


	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < Models.size() - 1; i++)
	{
		Models.at(i).model->Shutdown();
		delete Models.at(i).model;
		Models.at(i).model = 0;
	}

	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		delete m_Skybox;
		m_Skybox = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime)
{
	bool result;
	static float rotation = 0.0f;


	CamRot.y -= (PreX - mouseX) * 0.1f;
	CamRot.x -= (PreY - mouseY) * 0.1f;

	PreX = (float)mouseX;
	PreY = (float)mouseY;

	m_Camera->SetPosition(CamPos.x, CamPos.y, CamPos.z);
	m_Camera->SetRotation(CamRot.x, CamRot.y, CamRot.z);

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	//  스카이박스 설정
	m_Skybox->Frame(CamPos);

	//  Set the Screen Size.
	result = m_Text->SetScreen(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the location of the camera.
	result = m_Text->SetCameraPosition(CamPos.x, CamPos.y, CamPos.z, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//  Set the Objs Count.
	result = m_Text->SetObjs(Objs, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	//  Set the Polygon usage.
	result = m_Text->SetPolys(Polys, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
	D3DXMATRIX PlayerMatrix, ComMatrix, BallMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//  스카이박스
	m_Skybox->Render(m_D3D->GetDeviceContext(), m_D3D->GetRenderTarget(), m_D3D->GetDepthStencil(), viewMatrix, projectionMatrix);

	//  3차원 파트
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// Render the model using the light shader.

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);
	D3DXMatrixTranslation(&translateMatrix, -180.0f, 20.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	for (int i = 0; i < Models.size() - 1; i++)
	{
		D3DXMatrixTranslation(&translateMatrix, 60.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

		Models.at(i).model->Render(m_D3D->GetDeviceContext());
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), Models.at(i).model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			Models.at(i).model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixIdentity(&worldMatrix);
	//  2차원 파트
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::SetData(UINT input)
{
	//  Change Texture Mip Mapping Option
	switch (input)
	{
	case 0:  //  Numpad0 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 0);
		break;
	case 1:  //  Numpad1 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 1);
		break;
	case 2:  //  Numpad2 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 2);
		break;
	case 3:  //  Numpad3 Input
		m_LightShader->SetSampler(m_D3D->GetDevice(), 3);
		break;

	default:
		break;
	}
}

void GraphicsClass::GoForward()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0, 0, 1);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}
void GraphicsClass::GoLeft()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}
void GraphicsClass::GoBack()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0, 0, -1);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}
void GraphicsClass::GoRight()
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&Dir, CamRot.y * 0.0174532925f, CamRot.x * 0.0174532925f, CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	CamPos += Direction * speed;
}

void GraphicsClass::MoveUp()
{

}
void GraphicsClass::MoveDown()
{

}
void GraphicsClass::MoveLeft()
{

}
void GraphicsClass::MoveRight()
{

}