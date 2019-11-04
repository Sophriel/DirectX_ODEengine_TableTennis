////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Vector>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "OBJparser.h"
#include "textclass.h"
#include "skyboxclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////+
struct Model
{
	ModelClass* model;
	const char* filename;
	const WCHAR* texturename;

	D3DXVECTOR3 pos;
};

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, int, int, float);

	void SetData(UINT input);  //  밉매핑 방식 변경
	void GoForward();
	void GoLeft();
	void GoBack();
	void GoRight();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextClass* m_Text;
	
	SkyboxClass* m_Skybox;

	vector<Model> Models;
	Model GroundModel;
	Model PlayerModel, ComModel, BallModel;

	int Objs, Polys;

	D3DXVECTOR3 CamPos, CamRot;
	float speed = 10.0f;
	float PreX, PreY;
};

#endif