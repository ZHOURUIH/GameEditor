//-----------------------------------------------------------------------------
// File: mp.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES
#define MAGIC_PARTICLES

#include <string>
#include <vector>
#include <map>

#ifdef _ANDROID
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(_WINDOWS)
#include <glew.h>
#endif

#include "magic/magic.h"
#include "txObject.h"

enum TEXTURE_TYPE
{
	TT_PNG,
	TT_SWI,
};

class MP_Emitter;
class MP_Manager;
class MP_Atlas;
class MP_Copy;
class txMP_Device;
struct MP_POSITION;
struct MP_PTC;
class txCamera;
class txNode;

//#define MAGIC_INTERPOLATION_DEFAULT			0	// eng: interpolation mode is taken from emitter settings
//												// rus: режим интерполяции берется из эмиттера
//#define MAGIC_INTERPOLATION_ENABLE			1	// eng: interpolation is always enabled
//												// rus: всегда использовать интерполяцию
//#define MAGIC_INTERPOLATION_DISABLE			2	// eng: interpolation is always disabled
//												// rus: всегда отключать интерполяцию

enum MAGIC_CHANGE_MODE
{
	MCM_EMITTER_ONLY,				 // 当发射器的位置或朝向发生改变时,不改变已发射的粒子的位置和朝向
	MCM_EMITTER_AND_PARTICLES,		 // 当发射器的位置或朝向发生改变时,会改变已发射的粒子的位置和朝向,默认设置为这个值
};
//#define MAGIC_CHANGE_EMITTER_ONLY			0	// eng: preserve particle positions while changing emitter position or direction
//												// rus: при изменении позиции или направления эмиттера частицы остаются на прежнем месте
//#define MAGIC_CHANGE_EMITTER_AND_PARTICLES	1	// eng: move all the special effect when changing emitter position or direction
//												// rus: при изменении позиции или направления эмиттера весь спецэффект перемещается целиком
//#define MAGIC_CHANGE_EMITTER_DEFAULT		2	// eng: for each special effect setting specified in editor is used
//												// rus: не изменять настройку по умолчанию

// eng: Getting identity matrix 
// rus: Возвращает единичную матрицу
extern void MatrixIdentity(MAGIC_MATRIX* m);

// eng: Calculation of perspective projection matrix (analogue of D3DXMatrixPerspectiveFovLH from Direct3D) 
// rus: Расчет матрицы перспективной проекции (аналог D3DXMatrixPerspectiveFovLH из Direct3D)
extern void MatrixPerspectiveFovLH(MAGIC_MATRIX* m, float fovy, float aspect, float zn, float zf);

// eng: Calculation of orthogonal projection (analogue of D3DXMatrixOrthoLH from Direct3D) 
// rus: Расчет матрицы ортогональной проекции (аналог D3DXMatrixOrthoLH из Direct3D)
extern void MatrixOrthoLH(MAGIC_MATRIX* m, float width, float height, float zn, float zf);

// eng: Calculation of view matrix (analogue of D3DXMatrixLookAtLH from Direct3D) 
// rus: Расчет матрицы вида (аналог D3DXMatrixLookAtLH из Direct3D)
extern void MatrixLookAtLH(MAGIC_MATRIX* m, MP_POSITION* peye, MP_POSITION* pat, MP_POSITION* pup);

// eng: Calculation of perspective projection matrix (analogue of D3DXMatrixPerspectiveFovLH from Direct3D)
// rus: Расчет матрицы перспективной проекции (аналог D3DXMatrixPerspectiveFovLH из Direct3D)
extern void MatrixPerspectiveFovRH(MAGIC_MATRIX* m, float fovy, float aspect, float zn, float zf);

// eng: Calculation of orthogonal projection (analogue of D3DXMatrixOrthoLH from Direct3D)
// rus: Расчет матрицы ортогональной проекции (аналог D3DXMatrixOrthoLH из Direct3D)
extern void MatrixOrthoRH(MAGIC_MATRIX* m, float width, float height, float zn, float zf);

// eng: Calculation of view matrix (analogue of D3DXMatrixLookAtLH from Direct3D)
// rus: Расчет матрицы вида (аналог D3DXMatrixLookAtLH из Direct3D)
extern void MatrixLookAtRH(MAGIC_MATRIX* m, MP_POSITION* peye, MP_POSITION* pat, MP_POSITION* pup);

class MP_Platform
{
protected:
	std::string ptc_path;
	std::string texture_path;
	std::string temp_path;

	std::string wchar_t8;

public:
	static MP_Manager* MP;

public:
	MP_Platform();
	virtual ~MP_Platform();

	// eng: Returns time in milliseconds
	// rus: Возвращает время в милисекундах
	virtual unsigned long GetTick()=0;

	// eng: Returns path to folder with emitters
	// rus: Возвращает путь к папке с ptc-файлами
	virtual const char* GetPathToPTC()=0;

	// eng: Returns path to folder with textures
	// rus: Возвращает путь к папке с текстурами
	virtual const char* GetPathToTexture()=0;

	// eng: Returns path to folder which could be used by wrapper to store temporary files
	// rus: Возвращает путь к временной папке
	virtual const char* GetPathToTemp()=0;

	// eng: Finds the first ptc-file in emitters folder
	// rus: Поиск первого ptc-файла в папке с эмиттерами

	virtual const char* GetFirstFile(){return NULL;}
	// eng: Finds the next ptc-file in emitters folder
	// rus: Поиск очередного ptc-файла в папке с эмиттерами
	virtual const char* GetNextFile(){return NULL;}

	// eng: Converts wchar_t string into utf8
	// rus: Конвертирует строку типа wchar_t в строку типа utf8
	virtual const char* wchar_to_utf8(const wchar_t* str);

	// eng: Converts utf8 string into wchar_t
	// rus: Конвертирует строку типа utf8 в строку типа wchar_t
	virtual const wchar_t* utf8_to_wchar(const char* str);

	// eng: Opens ptc-file
	// rus: Открывает ptc-файл
	virtual HM_FILE OpenPTC(const char* ptc_file);

	// Deleting of file (analogue remove())
	// Удаление файла (аналог remove())
	virtual int RemoveFile(const char* file)=0;
};

// eng: Class that is used as storage for Magic Particles emitters
// rus: Класс, который является хранилищем эмиттеров Magic Particles
// 粒子文件的结构体
struct PtcStruct
{
	int mCpoySeed; // 拷贝种子,用于记录拷贝数量,生成拷贝后的发射器的后缀
	txMap<std::string, HM_EMITTER> mEmitterList; // 发射器列表
	PtcStruct()
	{
		mCpoySeed = 0;
		mEmitterList.clear();
	}
};

class MP_Manager
{
public:
	static TEXTURE_TYPE mTextureType;
protected:
	MP_Platform* platform;	// eng: object implementing OS specifics
							// rus: объект работающий со спецификой OS

	// eng: number of loaded emitters
	// rus: количество загруженных эмиттеров
	int k_emitter;
	int max_emitter;

	// eng: list of loaded emitters
	// rus: список загруженных эмиттеров
	MP_Emitter** m_emitter;
	HM_EMITTER* m_descriptor;

	// eng: number of loaded atlases
	// rus: количество загруженных атласов
	int k_atlas;

	// eng: list of loaded atlases
	// rus: список загруженных атласов
	MP_Atlas** m_atlas;

	// eng: number of files with particle copies
	// rus: количество файлов с копиями частиц
	int k_copy;

	// eng: list of files with particle copies
	// rus: список файлов с копиями частиц
	MP_Copy** m_copy;

	// eng: number of open files
	// rus: количество открытых файлов
	int k_ptc;

	// eng: list of open files
	// rus: список открытых файлов
	MP_PTC** m_ptc;

	// eng: settings to initialize emitters by default
	// rus: настройки для инициализации эмиттеров по умолчанию

	int atlas_width,atlas_height;
	int atlas_frame_step;
	float atlas_scale_step;

	bool copy_mode;				// eng: mode for working with files containing particle copies. false - file is deleted, when there are no emitters, that use it, true - file is deleted at program termination
								// rus: режим работы файлов с копиями частиц с копиями. false - файл удаляется, когда нет эмиттеров, которые его используют, true - файл удаляется при завершении работы программы

	HM_EMITTER next_descriptor;
	int next_index;

	bool is_new_atlas;

public:
	static txMP_Device* device;

public:
	MP_Manager();
	~MP_Manager()
	{
		Destroy();
	}

public:
	// eng: Cleaning up
	virtual void Destroy();

	// eng: Initialization
	void Initialization(int axis, MP_Platform* platform, int atlas_width=1024, int atlas_height=1024, int atlas_frame_step=1, float atlas_starting_scale=1.f, float atlas_scale_step=0.1f, bool copy_mode=true);

	// eng: Returning the number of emitters
	int GetEmitterCount(){return k_emitter;}

	// eng: Returning descriptor of first emitter. 
	HM_EMITTER GetFirstEmitter();

	// eng: Returning descriptor of next emitter. 
	HM_EMITTER GetNextEmitter(HM_EMITTER hmEmitter);

	// eng: Returning the emitter by its descriptor
	MP_Emitter* GetEmitter(HM_EMITTER hmEmitter);

	// eng: Returning the emitter by name
	MP_Emitter* GetEmitterByName(const char* name);

	// eng: Loading all emitters from emitters folder
	// 此函数暂不能使用
	void LoadAllEmitters(txMap<std::string, PtcStruct>& particleFileList);

	// eng: Loading all the emitters and animated folders from the file specified
	// file是ptc文件的路径,会加载ptc文件中的所有发射器,现在暂时认为一个ptc文件中只有一个粒子特效(可能会有多个发射器),将这一个粒子特效的所有发射器的节点挂到parent节点上
	// 返回的是加载的ptc文件中的所有发射器的句柄列表
	bool LoadEmittersFromFile(const char* file, txNode* parent, HM_FILE* pFileHandle, std::pair<std::string, PtcStruct>& filePair);

	HM_FILE LoadEmittersFromFileInMemory(char* address, txNode* parent, std::pair<std::string, PtcStruct>& filePair);

	// parent是发射器节点的父节点,emitter是返回加载的发射器的句柄
	HM_FILE LoadEmitterFromFile(const char* file, txNode* parent, HM_EMITTER* emitter);

	// eng: Deleting specified emitter 
	int DeleteEmitter(HM_EMITTER hmEmitter);

	// eng: Closing file
	int CloseFile(HM_FILE hmFile);

	// eng: Closing all files
	void CloseFiles();

	// eng: Duplicating specified emitter
	HM_EMITTER DuplicateEmitter(HM_EMITTER hmEmitter);

	// eng: Updating emitters taking into account the passed time
	int UpdateByTimer();

	// eng: Updating emitters
	// 并没有调用EmitterManager的更新函数,Emitter的更新是由它的节点来单独调用的
	// EmitterManager的更新函数中除了遍历发射器调用更新外,还有事件的调用,好像暂时没有用到事件
	void Update(double time);

	// eng: Rendering all emitters
	int Render(txCamera* camera);

	// eng: Stopping all the emitters
	void Stop();

	// eng: Refreshing textural atlases
	void RefreshAtlas();

	// eng: Restoring textural atlas in cases of loosing textures
	void RestoreAtlas();

	// eng: Returns the number of textural atlases
	int GetAtlasCount(){return k_atlas;}

	// eng: Returns textural atlas
	MP_Atlas* GetAtlas(int index){return m_atlas[index];}

	// eng: Adding new emitter into array
	void AddEmitter(MP_Emitter* emitter);

	// eng: Loading emitter
	// path是发射器在ptc文件中的路径
	MP_Emitter* LoadEmitter(HM_FILE file, const char* path, txNode* parent);

	// eng: Loading folder, 返回的是发射器句柄列表
	bool LoadFolder(HM_FILE file, const char* path, txNode* parent, std::pair<std::string, PtcStruct>& filePair);

	// eng: Adding file with particles copy
	MP_Copy* AddCopy(MP_Emitter* emitter);

	// eng: Deleting file with particles copy
	void DeleteCopy(MP_Copy* copy);

	// eng: Getting mode of working with files containing particle copies
	bool GetCopyMode(){return copy_mode;}

	// eng: Searching among files containing particle copies by specified emitter id
	MP_Copy* FindCopy(unsigned int emitter_id);

	// eng: Adding open file
	void AddPTC(MP_PTC* ptc_file);

	// eng: Deleting open file
	int DeletePTC(HM_FILE hmFile);

	// eng: Returns time in milliseconds
	unsigned long GetTick(){return platform->GetTick();}
	
	// eng: Returns path to folder with emitters
	const char* GetPathToPTC(){return platform->GetPathToPTC();}

	// eng: Returns path to folder with textures
	// rus: Возвращает путь к папке с текстурами
	const char* GetPathToTexture(){return platform->GetPathToTexture();}

	// eng: Returns path to folder which could be used by wrapper to store temporary files
	// rus: Возвращает путь к временной папке
	const char* GetPathToTemp(){return platform->GetPathToTemp();}

	// eng: Finds the first ptc-file in emitters folder
	// rus: Поиск первого ptc-файла в папке с эмиттерами
	const char* GetFirstFile(){return platform->GetFirstFile();}

	// eng: Finds the next ptc-file in emitters folder
	// rus: Поиск очередного ptc-файла в папке с эмиттерами
	const char* GetNextFile(){return platform->GetNextFile();}

	// eng: Converts wchar_t string into utf8
	// rus: Конвертирует строку типа wchar_t в строку типа utf8
	const char* wchar_to_utf8(const wchar_t* str){return platform->wchar_to_utf8(str);}

	// eng: Converts utf8 string into wchar_t
	// rus: Конвертирует строку типа utf8 в строку типа wchar_t
	const wchar_t* utf8_to_wchar(const char* str){return platform->utf8_to_wchar(str);}

	// eng: Opens ptc-file
	// rus: Открывает ptc-файл
	HM_FILE OpenPTC(const char* ptc_file){return platform->OpenPTC(ptc_file);}

	// Deleting of file (analogue remove())
	// Удаление файла (аналог remove())
	int RemoveFile(const char* file){return platform->RemoveFile(file);}
};

struct MP_POSITION
{
	float x,y,z;

	MP_POSITION();
	MP_POSITION(float x,float y,float z);
	MP_POSITION(float x,float y);

	MP_POSITION operator+(const MP_POSITION& pos);
	MP_POSITION operator+=(const MP_POSITION& pos);
	MP_POSITION operator-(const MP_POSITION& pos);
	MP_POSITION operator-=(const MP_POSITION& pos);

	float Length();
	
	void Normalize();
	
	// rus: Векторное произведение
	void Cross(const MP_POSITION* pV1, const MP_POSITION* pV2);
	// rus: Скалярное произведение
	float Dot(const MP_POSITION* p);
};


// ---------------------------------------------------------------------------------------

// eng: Class, specialized for work with the emitters
// rus: Класс, который хранит загруженные эмиттеры

enum EMITTER_STATE
{
	ES_STOP,		// emitter is not working
	ES_UPDATE,		// emitter is updated and rendered
	ES_INTERRUPT,	// emitter interrupts, i.e. is working while there are "alive" particles
	ES_VISIBLE		// Emitter is only rendered
};

enum EMITTER_UPDATE_MODE
{
	EUM_LOOP,  			// 一直循环更新发射器,直到设置为停止为止
	EUM_ONCE,			// 只更新一个周期,当发射器已经更新完成了一个周期就自动停止发射
};

class MP_Emitter : public txObject
{
protected:
	EMITTER_STATE mState;					// eng: state
	EMITTER_UPDATE_MODE mUpdateMode;
	MP_Manager* owner;			// eng: pointer to manager
								// eng: указатель на менеджер

	MP_POSITION mLastSettedPosition; // 上一次外部设置的位置
	MAGIC_DIRECTION mLasSettedtRotation; // 上一次外部设置的旋转
	float mLastSettedScale;	// 上一次外部设置的缩放

	HM_EMITTER emitter;			// eng: descriptor of emitter
								// rus:дескриптор эмиттера

	float z;					// eng: coordinate z of emitter
								// ru: координата z эмиттера

	bool first_restart;			// eng: indicates that emitter does not work
								// rus: признак того, что эмиттер еще не выставлялся на стартовую позицию анимации

	MAGIC_CHANGE_MODE mMagicChangeMode; // 当发射器的位置或方向发生改变时,是否需要将它已经发射出的粒子的位置和方向也作相应的改变
public:
	MP_Copy* copy;				// rus: file with particles copy
								// rus: файл с копией частиц

public:
	bool is_atlas;				// eng: indicates that atlas for this emitter was created
								// rus: признак того, что атлас для данного эмиттера уже был построен

	std::string restore_file;	// rus: file for restoring of emitter textures
								// rus: файл для восстановления текстур эмиттера

	static txMP_Device* device;
	
public:
	MP_Emitter(HM_EMITTER emitter, MP_Manager* owner);
	virtual ~MP_Emitter();

	virtual void NotifyParentUpdated(){}

	bool GetInterpolation();
	void SetInterpolation(bool value);
	void SetUpdateMode(const EMITTER_UPDATE_MODE& mode) { mUpdateMode = mode; }
	const EMITTER_UPDATE_MODE& GetUpdateMode() {return mUpdateMode;}
	void SetMagicChangeMode(const MAGIC_CHANGE_MODE& mode);
	const MAGIC_CHANGE_MODE& GetMagicChangeMode() { return mMagicChangeMode; }
	void SetParentNode(txNode* parent);
	// eng: Returns manager
	// eng: Возвращает менеджер
	MP_Manager* GetManager(){return owner;}

	// 该函数最好不要调用,节点存在问题
	MP_Emitter& operator=(const MP_Emitter& from);

	// eng: Returning the descriptor of the emitter to work with API
	// Возвращение дескриптора эмиттера, чтобы работать с API
	const HM_EMITTER& GetEmitter(){ return emitter; }

	// eng: Returning the name of the emitter
	// rus: Возвращение имени эмиттера
	const char* GetEmitterName();

	// eng: Restarting of emitter
	// rus: Установка эмиттера на стартовую позицию
	void Restart();

	// eng: Position of emitter
	// rus: Позиция эмиттера
	void GetPosition(MP_POSITION& position);
	void SetPosition(MP_POSITION& position); // 此函数执行结果正确,其他的设置发射器位置的函数可能执行结果不正确

	// eng: Moving the emitter to the position specified allowing restart. 
	// rus: Перемещение эмиттера в указанную позицию с возможностью перезапуска. При перемещении все существующие частицы перемещаются вместе с эмиттером
	void Move(MP_POSITION& position, const bool& restart = false);

	// eng: Offsetting the current emitter position by the value specified
	// rus: Смещение текущей позиции эмиттера на указанную величину. Будет использован текущий режим перемещения эмиттера
	void Offset(MP_POSITION& offset);

	// eng: Direction of emitter
	// rus: Направление эмиттера
	void SetDirection(MAGIC_DIRECTION* direction);
	void GetDirection(MAGIC_DIRECTION* direction);

	// eng: Setting the emitter direction to the specified value with the restart ability
	// rus: Поворот эмиттера в указанное направление с возможностью перезапуска. При повороте все существующие частицы поворачиваются вместе с эмиттером
	void Direct(MAGIC_DIRECTION* direction, const bool& restart = false);
	
	// eng: Rotating of the emitter by the specified value
	// rus: Вращение эмиттера на указанную величину. Будет использован текущий режим вращения эмиттера
	void Rotate(MAGIC_DIRECTION* offset);

	// eng: Returning the scale of the emitter
	// rus: Возвращение масштаба эмиттера
	float GetScale();

	// eng: Setting the scale of the emitter
	// rus: Установка масштаба эмиттера
	void SetScale(const float& scale);

	// eng: Returning the state of the emitter
	// rus: Возвращение статуса эмиттера
	const EMITTER_STATE& GetState(){ return mState; }

	// eng: Setting the state of the emitter
	// rus: Установка статуса эмиттера
	void SetState(const EMITTER_STATE& state);

	// eng: Updating emitter
	// rus: Обновление эмиттера
	virtual void Update(float fElaspedTime);

	// eng: Emitter visualization
	// rus: Отрисовка эмиттера. Возвращается количество нарисованных частиц
	int Render();
};

// eng: Class storing the files with particles copies
// rus: Класс, который хранит файлы с копиями частиц
class MP_Copy
{
protected:
	int reference;
	std::string file;
	HM_STREAM ram;

	unsigned int emitter_id;

	static int file_count;
	static std::string file_name;

public:
	static MP_Manager* MP;

public:
	MP_Copy(MP_Emitter* emitter);
	virtual ~MP_Copy();

	// eng: Cleaning up
	// rus: Очистка
	void Clear();

	// eng: Increasing of reference count
	// rus: Увеличение числа ссылок на файл
	void IncReference(MP_Emitter* emitter);
	// eng: Decreasing of reference count
	// rus: Уменьшение числа ссылок на файл
	void DecReference();

	// eng: Loading of particles from file to emitter
	// rus: Загрузка частиц из файла в эмиттер
	void LoadParticles(MP_Emitter* emitter);

	// rus: Returns of emitter ID, for which file was created
	// rus: Возвращает идентификатор эмиттера, для которого создан файл
	const unsigned int& GetEmitterID(){ return emitter_id; }
};

// eng: Class storing the textural atlas. This class will be abstract
// rus: Класс, который хранит текстурный атлас. Этот класс будет абстрактным
class MP_Atlas : public txEngineBase
{
protected:
	int atlas_width, atlas_height;
	std::string file_name;

public:
	static MP_Manager* MP;
	static txMP_Device* device;

public:
	MP_Atlas(const int& width, const int& height, const char* file);
	virtual ~MP_Atlas(){}

	// eng: Destroy atlas texture
	// rus: Уничтожить текстуру атласа
	virtual void Destroy() = 0;

	// eng: Returns sizes
	// rus: Возвращает размеры
	void GetSize(int& atlas_width,int& atlas_height)
	{
		atlas_width = this->atlas_width;
		atlas_height = this->atlas_height;
	}

	// eng: Returns file name of texture
	// rus: Возвращает имя файла текстуры
	const char* GetFile(){return file_name.c_str();}

	// eng: Loading of frame texture
	// rus: Загрузка текстурного кадра
	virtual void LoadTexture(const MAGIC_CHANGE_ATLAS* c) = 0;

	// eng: Cleaning up of rectangle
	// rus: Очистка прямоугольника
	virtual void CleanRectangle(const MAGIC_CHANGE_ATLAS*){}

	// eng: Beginning of emitter drawing
	// rus: Начало отрисовки эмиттера
	virtual void BeginDrawEmitter(MP_Emitter*){}

	// eng: End of emitter drawing
	// rus: Конец отрисовки эмиттера
	virtual void EndDrawEmitter(MP_Emitter*){}

	// eng: Beginning of particles drawing
	// rus: Начало отрисовки частиц
	virtual void BeginDrawParticles(int particles_count){}

	// eng: End of particles drawing
	// rus: Конец отрисовки частиц
	virtual void EndDrawParticles(){}

	// eng: Particle drawing
	// rus: Отрисовка частицы
	virtual void Draw(MAGIC_PARTICLE_VERTEXES* vertex_rectangle) = 0;

	// eng: Setting of intense
	// rus: Установка интенсивности
	virtual void SetIntense(const bool& intense) = 0;
};

struct MP_PTC
{
	HM_FILE hmFile;
	HM_STREAM hmStream;
	char* data;

	MP_PTC();
	MP_PTC(const MP_PTC& other);
	MP_PTC& operator = (MP_PTC& other);
	virtual ~MP_PTC();
	int Close();
};

struct MP_VIEWPORT
{
	unsigned int x,y;
	unsigned int width, height;
	float min_z,max_z;
};

// eng: Class controlling drawing. This class will be abstract
// rus: Класс, который управляет рисованием. Этот класс будет абстрактным
class MP_Device : public txEngineBase
{
protected:

public:
	static MP_Manager* MP;

public:
	MP_Device();
	virtual ~MP_Device();

	// eng: Creating
	// rus: Создание
	virtual bool Create() = 0;

	// eng: Destroying
	// rus: Уничтожение
	virtual void Destroy();

	// eng: Beginning of scene drawing
	// rus: Начало отрисовки сцены
	virtual void BeginScene(txCamera* txcamera);

	// eng: End of scene drawing
	// rus: Конец отрисовки сцены
	virtual void EndScene() = 0;

	// eng: Indicates that device is lost
	// rus: Проверка на потерю устройства рисования
	virtual bool IsLost() = 0;

	// eng: Setting of scene
	// rus: Настройка сцены
#ifdef MAGIC_3D
	virtual void SetScene3d() = 0;
#else
	virtual void SetScene3d(){}
#endif

	// eng: Creating of atlas object
	// rus: Создание объекта атласа
	virtual MP_Atlas* NewAtlas(const int& width, const int& height, const char* file);
};

// ---------------------------------------------------------------------------------------

// eng: Saves TGA for testing
// rus: Сохраняет TGA для тестирования
extern void SaveTGA(const short& w, const short& h, unsigned char* pixel, const char* file);

#endif
