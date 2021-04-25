#pragma once
#include<iostream>
#include <vector>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// TODO list
// 1. rewrite code for dx11
// 2. rewrite code with many good classes
// 3. add camera control
// 4. add texture loading
// 5. add different figures
// 6. make new TODO list


// struct point (3d vector with color)
struct Point
{
	float x, y, z;
	DWORD c;
	Point():x(0.0), y(0.0),z(0.0), c(0x00ff0000){}
	Point(float x_, float y_, float z_, DWORD c_) :x(x_), y(y_), z(z_), c(c_) {}
};

// struct surface (consist 4 points)
struct Surface
{
	Point p[4];

	Surface() {}
	Surface(Point p0, Point p1, Point p2, Point p3) : p{p0, p1,p2,p3} {}
};

// struct texture point (has coordinates u and v)
struct PointTex
{
	float x, y, z;
	float u, v;
};

// class box (cube - has 6 surfaces)
class Box
{
public:
	Surface s[6];

	Box():
		Box(Point())
	{}

	Box(Point c_) // we set central point of cube (size of cube is 1)
	{
		s[0] = Surface // for this example sides of cube have red green and blue colours
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z - 0.5, 0x0000ff00), 
			Point(c_.x - 0.5, c_.y - 0.5, c_.z + 0.5, 0x0000ff00), // green
			Point(c_.x + 0.5, c_.y - 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z - 0.5, 0x0000ff00)
		);
		s[1] = Surface
		(
			Point(c_.x - 0.5, c_.y + 0.5, c_.z - 0.5, 0x0000ff00),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z - 0.5, 0x0000ff00)
		);

		s[2] = Surface
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z - 0.5, 0x00ff0000),
			Point(c_.x - 0.5, c_.y - 0.5, c_.z + 0.5, 0x00ff0000),  // red
			Point(c_.x - 0.5, c_.y + 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z - 0.5, 0x00ff0000)
		);

		s[3] = Surface
		(
			Point(c_.x + 0.5, c_.y - 0.5, c_.z - 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z - 0.5, 0x00ff0000)
		);

		s[4] = Surface
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z - 0.5, 0x000000ff),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z - 0.5, 0x000000ff),  // blue
			Point(c_.x + 0.5, c_.y + 0.5, c_.z - 0.5, 0x000000ff),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z - 0.5, 0x000000ff)
		);

		s[5] = Surface
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z + 0.5, 0x000000ff),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z + 0.5, 0x000000ff),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z + 0.5, 0x000000ff),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z + 0.5, 0x000000ff)
		);
	}
};

// class triangle for rendering 
class Tri
{
public:
	Point p[3];
	Tri()
	{
		Point c;
		p[0].x = c.x - 1.5;  p[0].y = 0 ; p[0].z = c.z - 1.5; p[0].c = 0x0000ff00;
		p[1].x = c.x + 1.5;  p[1].y = 0 ; p[1].z = c.z + 1.5; p[1].c = 0x00ffff00;
		p[2].x = 0 ;  p[2].y = c.y + 2.5; p[2].z = 0; p[2].c = 0x0000ffff;
	}
};


// main class for rendering graphic for direcx 3d
class Renderer
{
public:
	// it's our textures of block (box) textures from minecraft
	IDirect3DTexture9* texTop;
	IDirect3DTexture9* texSide;
	IDirect3DTexture9* texBottom;

	D3DMATERIAL9 mat; // not very good way to add materials...

	std::vector<Box*> block; // may be one day there will be many blocks....

	// vertexes and indices buffers
	LPDIRECT3DVERTEXBUFFER9 vertexBuff = NULL; 
	LPDIRECT3DINDEXBUFFER9 indBuff = NULL;
	LPDIRECT3DVERTEXBUFFER9 vertexTexBuff = NULL;

	int nVertex; 
	int nInd;
	int nTri;

	// devices
	IDirect3D9* d3d = nullptr;
	IDirect3DDevice9* device = nullptr;

	float viewAngle = 0.0; // angle of world matrix

	// resolution and windowed mode
	bool windowed = true;
	int width = 800;
	int height = 600;

	// handler of main window
	HWND hWnd = NULL;

	Renderer()
	{}

	void init(HWND& hWnd_)
	{
		// standart way....
		hWnd = hWnd_;
		if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
		{
			std::cout << "Error initialization directX\n";
			return;
		}
		D3DPRESENT_PARAMETERS presentParams;
		ZeroMemory(&presentParams, sizeof(presentParams));
		presentParams.BackBufferWidth = width;
		presentParams.BackBufferHeight = height;
		presentParams.AutoDepthStencilFormat = D3DFMT_D16;
		presentParams.EnableAutoDepthStencil = true;
		RECT wndRect;
		RECT clientRect;

		GetWindowRect(hWnd, &wndRect);
		GetClientRect(hWnd, &clientRect);

		MoveWindow(hWnd,wndRect.left, wndRect.top,width, height, TRUE);

		D3DDISPLAYMODE displayMode;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
		presentParams.BackBufferFormat = displayMode.Format;
		presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentParams.Windowed = TRUE;

		DWORD flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		d3d->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			flags,
			&presentParams,
			&device
		);
		// use lighting and depth buffer
		device->SetRenderState(D3DRS_LIGHTING, TRUE);  
		device->SetRenderState(D3DRS_ZENABLE, TRUE);

		// projection matrix
		D3DXMATRIX matrixProjection;
		float aspect = width / height;
		float minDist = 0.1; // min and max distance for camera proection
		float maxDist = 1000.0;
		D3DXMatrixPerspectiveFovLH(&matrixProjection, D3DX_PI / 4.0, aspect, minDist, maxDist);
		device->SetTransform(D3DTS_PROJECTION, &matrixProjection);

		loadTextures();

		device->CreateVertexBuffer // load vertex buffer
		(
			10000 * sizeof(Point), // reserve 10000 points
			0,
			D3DFVF_XYZ | D3DFVF_DIFFUSE,
			D3DPOOL_DEFAULT,         
			&vertexBuff,           // address of vertex buffer
			NULL
		);
		
		device->CreateIndexBuffer // load indices buffer
		(
			sizeof(short) * 10000, // reserve 10000 shorts
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&indBuff, // to indices buffer
			0
		);
		// and buffer of texture points
		device->CreateVertexBuffer
		(
			10000 * sizeof(PointTex), // reserve 10000 points
			D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_TEX1,		  
			D3DPOOL_DEFAULT,         
			&vertexTexBuff,        
			NULL
		);

		ZeroMemory(&mat, sizeof(D3DMATERIAL9));
		mat.Diffuse.r = 255;
		mat.Diffuse.g = 255;
		mat.Diffuse.b = 255;
		mat.Diffuse.a = 255;
		mat.Ambient = mat.Diffuse;

		
	}
	
	void loadTextures()
	{ // it is textures of minecraft grass. but you can put either images for bottom top and sides
		D3DXCreateTextureFromFile 
		(
			device,
			L"bottom.png",
			&texBottom
		);

		D3DXCreateTextureFromFile
		(
			device,
			L"side.png",
			&texSide
		);

		D3DXCreateTextureFromFile
		(
			device,
			L"top.png",
			&texTop
		);

	}

	~Renderer()
	{ //  may be we need kill buffer of textures and other
		if (device)
		{
			device->Release();
			device = nullptr;
		}
		if (d3d)
		{
			d3d->Release();
			d3d = nullptr;
		}
	}

	// add box...
	void addBox(Point c)
	{
		int nVertexNew = 24;
		int nIndNew = 36;
		int nTriNew = 12;
		//Box box(Point(0, 0, 0, 0xffff0000));

		block.push_back(new Box(c));

		auto& box = block.back();
		//Tri tri;

		void* pBuff = NULL;
		// copy points
		vertexBuff->Lock(0, sizeof(Point) * 6 * 4, &pBuff, 0);
		memcpy(pBuff, box->s, 6 * 4 * sizeof(Point));
		vertexBuff->Unlock();
		
		// it is indices of points for surfaces
		const unsigned short ind[] = 
		{
			0, 2, 1, 0, 3, 2, // red xx
			4, 5, 6, 4, 6, 7, // red
			8, 9,10, 8,10,11, // green
		   13,12,14,14,12,15, // green
		   17,16,18,18,16,19, // blue
		   20,21,22,20,22,23  // blue
		}; // sorry but box colorless because light....

		// indices buffer
	    indBuff->Lock(0, sizeof(short) * nIndNew, &pBuff, 0);
		memcpy(pBuff, ind, nIndNew * sizeof(short));
		indBuff->Unlock();

		// add count of triangles and points to total count
		nTri += 12;
		nVertex += nVertexNew;

		// bottom
		PointTex texPoint[4];

		texPoint[0].x = 1.0; texPoint[0].y = 0.0; texPoint[0].z = 0.0; texPoint[0].u = 0.0; texPoint[0].v = 0.0;
		texPoint[1].x = 1.0; texPoint[1].y = 0.0; texPoint[1].z = 1.0; texPoint[1].u = 1.0; texPoint[1].v = 0.0;
		texPoint[2].x = 0.0; texPoint[2].y = 0.0; texPoint[2].z = 0.0; texPoint[2].u = 0.0; texPoint[2].v = 1.0;
		texPoint[3].x = 0.0; texPoint[3].y = 0.0; texPoint[3].z = 1.0; texPoint[3].u = 1.0; texPoint[3].v = 1.0;

		vertexTexBuff->Lock(0, sizeof(PointTex)* 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();

		// side z = 1
		texPoint[0].x = 1.0; texPoint[0].y = 1.0; texPoint[0].z = 1.0; texPoint[0].u = 0.0; texPoint[0].v = 0.0;
		texPoint[1].x = 0.0; texPoint[1].y = 1.0; texPoint[1].z = 1.0; texPoint[1].u = 1.0; texPoint[1].v = 0.0;
		texPoint[2].x = 1.0; texPoint[2].y = 0.0; texPoint[2].z = 1.0; texPoint[2].u = 0.0; texPoint[2].v = 1.0;
		texPoint[3].x = 0.0; texPoint[3].y = 0.0; texPoint[3].z = 1.0; texPoint[3].u = 1.0; texPoint[3].v = 1.0;

		vertexTexBuff->Lock(sizeof(PointTex) * 4, sizeof(PointTex) * 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();

		// side z = 0
		texPoint[0].x = 1.0; texPoint[0].y = 0.0; texPoint[0].z = 0.0; texPoint[0].u = 0.0; texPoint[0].v = 1.0;
		texPoint[1].x = 0.0; texPoint[1].y = 0.0; texPoint[1].z = 0.0; texPoint[1].u = 1.0; texPoint[1].v = 1.0;
		texPoint[2].x = 1.0; texPoint[2].y = 1.0; texPoint[2].z = 0.0; texPoint[2].u = 0.0; texPoint[2].v = 0.0;
		texPoint[3].x = 0.0; texPoint[3].y = 1.0; texPoint[3].z = 0.0; texPoint[3].u = 1.0; texPoint[3].v = 0.0;

		vertexTexBuff->Lock(sizeof(PointTex) * 4 * 2, sizeof(PointTex) * 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();
		
		// side x= 0
		texPoint[0].x = 0.0; texPoint[0].y = 0.0; texPoint[0].z = 0.0; texPoint[0].u = 0.0; texPoint[0].v = 1.0;
		texPoint[1].x = 0.0; texPoint[1].y = 0.0; texPoint[1].z = 1.0; texPoint[1].u = 1.0; texPoint[1].v = 1.0;
		texPoint[2].x = 0.0; texPoint[2].y = 1.0; texPoint[2].z = 0.0; texPoint[2].u = 0.0; texPoint[2].v = 0.0;
		texPoint[3].x = 0.0; texPoint[3].y = 1.0; texPoint[3].z = 1.0; texPoint[3].u = 1.0; texPoint[3].v = 0.0;

		vertexTexBuff->Lock(sizeof(PointTex) * 4 * 3, sizeof(PointTex) * 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();

		// side x= 1
		texPoint[0].x = 1.0; texPoint[0].y = 0.0; texPoint[0].z = 1.0; texPoint[0].u = 0.0; texPoint[0].v = 1.0;
		texPoint[1].x = 1.0; texPoint[1].y = 0.0; texPoint[1].z = 0.0; texPoint[1].u = 1.0; texPoint[1].v = 1.0;
		texPoint[2].x = 1.0; texPoint[2].y = 1.0; texPoint[2].z = 1.0; texPoint[2].u = 0.0; texPoint[2].v = 0.0;
		texPoint[3].x = 1.0; texPoint[3].y = 1.0; texPoint[3].z = 0.0; texPoint[3].u = 1.0; texPoint[3].v = 0.0;

		vertexTexBuff->Lock(sizeof(PointTex) * 4 * 4, sizeof(PointTex) * 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();

		// top
		texPoint[0].x = 0.0; texPoint[0].y = 1.0; texPoint[0].z = 0.0; texPoint[0].u = 0.0; texPoint[0].v = 0.0;
		texPoint[1].x = 0.0; texPoint[1].y = 1.0; texPoint[1].z = 1.0; texPoint[1].u = 1.0; texPoint[1].v = 0.0;
		texPoint[2].x = 1.0; texPoint[2].y = 1.0; texPoint[2].z = 0.0; texPoint[2].u = 0.0; texPoint[2].v = 1.0;
		texPoint[3].x = 1.0; texPoint[3].y = 1.0; texPoint[3].z = 1.0; texPoint[3].u = 1.0; texPoint[3].v = 1.0;

		vertexTexBuff->Lock(sizeof(PointTex) * 4 * 5, sizeof(PointTex) * 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();

		// move camera to (2,0,10)
		setView();

	}

	void setView()
	{
		D3DMATRIX view =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			2,0,10,1,
		};
		device->SetTransform(D3DTS_VIEW, &view);
	}

	void draw()
	{
		device->Clear(1, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		device->BeginScene();

		viewAngle += 0.01;
		float b = viewAngle;


		// turn world matrix in time
		D3DMATRIX World =
		{
			cos(b) * cos(b), cos(b) * sin(b), sin(b), 0,
			-sin(b), cos(b), 0, 0,
			-sin(b) * cos(b), -sin(b) * sin(b), cos(b), 0,
			0, 0, 0, 1 
		};

		// we make a light!
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_POINT;
		light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = light.Diffuse.a = 1.0;
		light.Ambient = light.Diffuse;
		light.Specular = light.Diffuse;
		light.Range = 40.0;
		light.Attenuation0 = 0.0;
		light.Attenuation1 = 0.0;
		light.Attenuation2 = 30.0;


		light.Position = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
		//	light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

		device->SetLight(0, &light);
		device->LightEnable(0, TRUE);
		device->SetRenderState(D3DRS_LIGHTING, TRUE);

		// set it
		device->SetTransform(D3DTS_WORLD, &World);

		device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		
		device->SetMaterial(&mat);

		device->SetStreamSource(0, vertexBuff, 0, sizeof(Point));
		device->SetIndices(indBuff);
		/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// draw our triangles. alas, but they are not colored because i make wrong light material for boxes
		device->DrawIndexedPrimitive
		(
			D3DPT_TRIANGLELIST,
			0,
			0,
			nVertex,
			0,
			nTri
		);
		
		// drawing box with texture minecraft grass
		
		device->SetTexture(0, texBottom);
		device->SetStreamSource(0, vertexTexBuff, 0, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

		device->SetTexture(0, texSide);
		device->SetStreamSource(0, vertexTexBuff, sizeof(PointTex) * 4 *1, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
		device->SetTexture(0, texSide);
		device->SetStreamSource(0, vertexTexBuff, sizeof(PointTex)*4*2, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		device->SetTexture(0, texSide);
		device->SetStreamSource(0, vertexTexBuff, sizeof(PointTex) * 4*3, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		device->SetTexture(0, texSide);
		device->SetStreamSource(0, vertexTexBuff, sizeof(PointTex) * 4 * 4, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		device->SetTexture(0, texTop);
		device->SetStreamSource(0, vertexTexBuff, sizeof(PointTex) * 4 * 5, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->EndScene();
		device->Present(0, 0, 0, 0);
	}

};

