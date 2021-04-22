#pragma once
#include <vector>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// класс точка (вектор)
struct Point
{
	float x, y, z;
	DWORD c;
	Point():x(0.0), y(0.0),z(0.0), c(0x00ff0000){}
	Point(float x_, float y_, float z_, DWORD c_) :x(x_), y(y_), z(z_), c(c_) {}
};

// класс поверхность (4 точки)
struct Surface
{
	Point p[4];

	Surface() {}
	Surface(Point p0, Point p1, Point p2, Point p3) : p{p0, p1,p2,p3} {}
};

/* должен был бы быть класс точка текстуры
struct PointTex
{
	float x, y, z;
	float u, v;
};
*/
// класс коробка (кубик)
class Box
{
public:
	Surface s[6];

	Box():
		Box(Point())
	{}

	Box(Point c_) // неного кривой способ задания поверхностей
	{
		s[0] = Surface
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z - 0.5, 0x00ff0000), 
			Point(c_.x - 0.5, c_.y - 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z - 0.5, 0x00ff0000)
		);
		s[1] = Surface
		(
			Point(c_.x - 0.5, c_.y + 0.5, c_.z - 0.5, 0x00ff0000),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z + 0.5, 0x00ff0000),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z - 0.5, 0x00ff0000)
		);

		s[2] = Surface
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z - 0.5, 0x0000ff00),
			Point(c_.x - 0.5, c_.y - 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x - 0.5, c_.y + 0.5, c_.z - 0.5, 0x0000ff00)
		);

		s[3] = Surface
		(
			Point(c_.x + 0.5, c_.y - 0.5, c_.z - 0.5, 0x0000ff00),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z + 0.5, 0x0000ff00),
			Point(c_.x + 0.5, c_.y + 0.5, c_.z - 0.5, 0x0000ff00)
		);

		s[4] = Surface
		(
			Point(c_.x - 0.5, c_.y - 0.5, c_.z - 0.5, 0x000000ff),
			Point(c_.x + 0.5, c_.y - 0.5, c_.z - 0.5, 0x000000ff),
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

/* класс треугольник для отображения примитива
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
*/

// главный класс для работы с графикой
class Renderer
{
public:
	// материалы и текстуры для вывода текстур в будуще
	//D3DMATERIAL9* material; 
	//IDirect3DTexture9* texTop;
	//IDirect3DTexture9* texSide;
	//IDirect3DTexture9* texBottom;

	//std::vector<Box*> block; // в будущем тут будет много блоков как в майнкрафте

	// буферы вершин индексов и вершин текстур
	LPDIRECT3DVERTEXBUFFER9 vertexBuff = NULL; 
	LPDIRECT3DINDEXBUFFER9 indBuff = NULL;
	//LPDIRECT3DVERTEXBUFFER9 vertexTexBuff = NULL;

	int nVertex; 
	int nInd;
	int nTri;
//	std::vector <float3> point;
	//std::vec

	IDirect3D9* d3d = nullptr;
	IDirect3DDevice9* device = nullptr;

	float viewAngle = 0.0; //угол поворота мировой атрицы

	bool windowed = true;
	int width = 800;
	int height = 600;

	HWND hWnd = NULL;

	Renderer()
	{}

	void init(HWND& hWnd_)
	{
		// все стандартно...
		hWnd = hWnd_;
		if((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
			return ;

		D3DPRESENT_PARAMETERS presentParams;
		ZeroMemory(&presentParams, sizeof(presentParams));
		presentParams.BackBufferWidth = width;
		presentParams.BackBufferHeight = height;

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
		// без освещения и глубины (z)
		device->SetRenderState(D3DRS_LIGHTING, FALSE);  
		device->SetRenderState(D3DRS_ZENABLE, FALSE);

		// матрица проекции 
		D3DXMATRIX matrixProjection;
		float aspect = width / height;
		D3DXMatrixPerspectiveFovLH(&matrixProjection, D3DX_PI / 4.0, aspect, 0.1, 1000.0);
		device->SetTransform(D3DTS_PROJECTION, &matrixProjection);

	//	loadTextures(); загрузка текстур будет тут

		device->CreateVertexBuffer // грузим вершинный буфер, точек побольше
		(
			10000 * sizeof(Point), // резерв в 10000 точек
			0,
			D3DFVF_XYZ | D3DFVF_DIFFUSE,		  // Буфер будет хранить координаты XYZ
			D3DPOOL_DEFAULT,          // Размещение в пуле по умолчанию
			&vertexBuff,           // Указатель на объект, куда будем помещен буфер
			NULL
		);
		
		device->CreateIndexBuffer // грузим буфер индексов
		(
			sizeof(short) * 10000, //резерв 10000 шортов
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&indBuff,
			0
		);
		/* должны были бы грузить буфер вершин текстур
		device->CreateVertexBuffer
		(
			10000 * sizeof(PointTex), // Необходимое количество байт
			D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_TEX1,		  // Буфер будет хранить координаты XYZ
			D3DPOOL_DEFAULT,          // Размещение в пуле по умолчанию
			&vertexTexBuff,           // Указатель на объект, куда будем помещен буфер
			NULL
		);
		*/
	}
	/*
	void loadTextures()
	{ // здесь грузим текстуры травы из майнкрафта...
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

	}*/

	~Renderer()
	{
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

	// добавляем ящик 
	void addBox(Point c)
	{
		int nVertexNew = 24;
		int nIndNew = 36;
		int nTriNew = 12;
		Box box(Point(0, 0, 0, 0xffff0000));

		//block.push_back(new Box(c));

	//	auto& box = block.back();
		//Tri tri;

		void* pBuff = NULL;
		// собираем буфер вершин
		vertexBuff->Lock(0, sizeof(Point) * 6 * 4, &pBuff, 0);
		memcpy(pBuff, box.s, 6 * 4 * sizeof(Point));
		vertexBuff->Unlock();
		
		// здесь сидят сарфейсы
		const unsigned short ind[] = 
		{
			0, 2, 1, 0, 3, 2, // red xx
			4, 5, 6, 4, 6, 7, // red
			8, 9,10, 8,10,11, // green
		   13,12,14,14,12,15, // green
		   17,16,18,18,16,19, // blue
		   20,21,22,20,22,23  // blue
		};
		
	//	const unsigned short ind[] = { 0,1,2,3,1,2 };
	//	int nIndNew = 6;

		// собираем буфер индексов
	    indBuff->Lock(0, sizeof(short) * nIndNew, &pBuff, 0);
		memcpy(pBuff, ind, nIndNew * sizeof(short));
		indBuff->Unlock();

		// запоминаем сколько добавили треугольников вершин индексов (в будущем понадобится)
		nTri += 12;
		nVertex += nVertexNew;

		// код внизу - неудавшаяся попытка прикрутить текстуры к сторонам ящика
		/*PointTex texPoint[4];
		texPoint[0].x = 0.0; texPoint[0].y = 0.0; texPoint[0].z = 0.0; texPoint[0].u = 0.0; texPoint[0].v = 0.0;
		texPoint[1].x = 1.0; texPoint[1].y = 0.0; texPoint[1].z = 0.0; texPoint[1].u = 1.0; texPoint[1].v = 0.0;
		texPoint[2].x = 1.0; texPoint[2].y = 1.0; texPoint[2].z = 0.0; texPoint[2].u = 1.0; texPoint[2].v = 1.0;
		texPoint[3].x = 0.0; texPoint[3].y = 1.0; texPoint[3].z = 0.0; texPoint[3].u = 0.0; texPoint[3].v = 1.0;

		vertexTexBuff->Lock(0, sizeof(PointTex)* 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();

		texPoint[0].x = 0.0; texPoint[0].y = 0.0; texPoint[0].z = 1.0; texPoint[0].u = 1.0; texPoint[0].v = 0.0;
		texPoint[1].x = 1.0; texPoint[1].y = 0.0; texPoint[1].z = 1.0; texPoint[1].u = 0.0; texPoint[1].v = 1.0;
		texPoint[2].x = 1.0; texPoint[2].y = 1.0; texPoint[2].z = 1.0; texPoint[2].u = 0.0; texPoint[2].v = 0.0;
		texPoint[3].x = 0.0; texPoint[3].y = 1.0; texPoint[3].z = 1.0; texPoint[3].u = 1.0; texPoint[3].v = 1.0;

		vertexTexBuff->Lock(sizeof(PointTex) * 4, sizeof(PointTex) * 4, &pBuff, 0);
		memcpy(pBuff, texPoint, 4 * sizeof(PointTex));
		vertexTexBuff->Unlock();
		*/

		// смещаем вид
		setView();

	}

	void setView()
	{
		D3DMATRIX view =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,10,1,
		};
		device->SetTransform(D3DTS_VIEW, &view);
	}

	void rend()
	{
		device->Clear(1, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		device->BeginScene();

		viewAngle += 0.01;
		float b = viewAngle;
		
		// со временем крутим мировую матрицу
		D3DMATRIX World =
		{
			cos(b) * cos(b), cos(b) * sin(b), sin(b), 0,
			-sin(b), cos(b), 0, 0,
			-sin(b) * cos(b), -sin(b) * sin(b), cos(b), 0,
			0, 0, 0, 1 
		};

		// устанавливаем ее
		device->SetTransform(D3DTS_WORLD, &World);

		device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		
		device->SetStreamSource(0, vertexBuff, 0, sizeof(Point));
		device->SetIndices(indBuff);
		// выводим наши треугольнички
		device->DrawIndexedPrimitive
		(
			D3DPT_TRIANGLELIST,
			0,
			0,
			nVertex,
			0,
			nTri
		);
		// здесь должен был быть вывод текстур на сторонах, но он не получился
		/*
		device->SetTexture(0, texTop);
		device->SetStreamSource(0, vertexTexBuff, 0, sizeof(PointTex));
		device->SetVertexShader(0);
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
		*/
	//	device->SetTexture(0, texBottom);
	//	device->SetStreamSource(0, vertexTexBuff, sizeof(PointTex)*4, sizeof(PointTex));
	//	device->SetVertexShader(0);
	//	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	//	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		device->EndScene();
		device->Present(0, 0, 0, 0);
	}

};

