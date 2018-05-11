//-----------------------------------------------------------------------------
// File: mp.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include "txMP_wrap.h"
#include "txMP_Device.h"
#include "txMP_Atlas.h"
#include "txNode.h"
#include "txOpenGLBuffer.h"
#include "txNodeManager.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"
#include "magic/magic.h"

#include <errno.h>

#if defined(_WINDOWS) || defined(WINAPI_FAMILY)
#pragma warning ( disable : 4996)
#endif

#ifdef MAGIC_3D

// ru: Инверсия кватерниона
// en: Quaternion inversion
void MagicQuaternionInversion(MAGIC_DIRECTION* q)
{
	float N = (float)(sqrt(q->x*q->x + q->y*q->y + q->z*q->z + q->w*q->w));
	q->x = -q->x / N;
	q->y = -q->y / N;
	q->z = -q->z / N;
	q->w = q->w / N;
}

// ru: Перемножение кватернионов
// en: Quaternion multiplication
void MagicQuaternionMul(MAGIC_DIRECTION *res, const MAGIC_DIRECTION *q1, const MAGIC_DIRECTION *q2)
{
	float A, B, C, D, E, F, G, H;

	A = (q1->w + q1->x) * (q2->w + q2->x);
	B = (q1->z - q1->y) * (q2->y - q2->z);
	C = (q1->x - q1->w) * (q2->y + q2->z);
	D = (q1->y + q1->z) * (q2->x - q2->w);
	E = (q1->x + q1->z) * (q2->x + q2->y);
	F = (q1->x - q1->z) * (q2->x - q2->y);
	G = (q1->w + q1->y) * (q2->w - q2->z);
	H = (q1->w - q1->y) * (q2->w + q2->z);

	res->w = B + (-E - F + G + H) * 0.5f;
	res->x = A - (E + F + G + H) * 0.5f;
	res->y = -C + (E - F + G - H) * 0.5f;
	res->z = -D + (E - F - G + H) * 0.5f;
}

#endif

// ------------------------------------------------------------------------------------------

// ru: Событие "Создание частицы"
// en: Event "Creating of particle"
void EventCreation(MAGIC_EVENT* evt)
{
	// ...
	// ...
	// ...
}

// ru: Событие "Уничтожение частицы"
// en: Event "Destruction of particle"
void EventDestruction(MAGIC_EVENT* evt)
{
	// ...
	// ...
	// ...
}

// ru: Событие "Существование частицы"
// en: Event "Existence of particle"
void EventExistence(MAGIC_EVENT* evt)
{
	// ...
	// ...
	// ...
}

// ru: Событие "Столкновение с препятствием"
// en: Event "Collision of particle with an obstacle"
void EventCollision(MAGIC_EVENT* evt)
{
	// ...
	// ...
	// ...
}

// ru: Событие "Примагничивание частицы"
// en: Event "Attraction of particle to the magnet"
void EventMagnetism(MAGIC_EVENT* evt)
{
	// ...
	// ...
	// ...
}

void(*function_event[5])(MAGIC_EVENT* evt) = { EventCreation, EventDestruction, EventExistence, EventCollision, EventMagnetism };

MP_Manager* MP_Platform::MP = NULL;

MP_Platform::MP_Platform()
{
}

MP_Platform::~MP_Platform()
{
}

// ru: Конвертация wchar_t в utf8
// en: Conversion of wchar_t in utf8
const char* MP_Platform::wchar_to_utf8(const wchar_t* str)
{
	const char* str8 = NULL;

	int size = sizeof(wchar_t);
	switch (size)
	{
	case 1:
		wchar_t8 = (const char*)str;
		str8 = wchar_t8.c_str();
		break;

	case 2:
		str8 = (const char*)Magic_UTF16to8((const unsigned short*)str);
		break;

	case 4:
		str8 = (const char*)Magic_UTF32to8((const unsigned int*)str);
		break;
	}
	return str8;
}

// ru: Конвертация utf8 в wchar_t
// en: Conversion of utf8 in wchar_t
const wchar_t* MP_Platform::utf8_to_wchar(const char* str)
{
	wchar_t* strw = NULL;

	int size = sizeof(wchar_t);
	switch (size)
	{
	case 1:
		wchar_t8 = str;
		strw = (wchar_t*)wchar_t8.c_str();
		break;

	case 2:
		strw = (wchar_t*)Magic_UTF8to16((const unsigned char*)str);
		break;

	case 4:
		strw = (wchar_t*)Magic_UTF8to32((const unsigned char*)str);
		break;
	}

	return strw;
}

// eng: Opens ptc-file
// rus: Открывает ptc-файл
HM_FILE MP_Platform::OpenPTC(const char* ptc_file)
{
	HM_STREAM hmStream = Magic_StreamOpenFile(ptc_file, MAGIC_STREAM_READ);
	HM_FILE hmFile = Magic_OpenStream(hmStream);
	if (hmFile > 0)
	{
		MP_PTC* ptc = TRACE_NEW(MP_PTC, ptc);
		ptc->hmFile = hmFile;
		ptc->hmStream = hmStream;
		MP->AddPTC(ptc);
	}
	else
	{
		Magic_StreamClose(hmStream);
	}
	return hmFile;
}

// ------------------------------------------------------------------------------------------

MP_POSITION::MP_POSITION()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

MP_POSITION::MP_POSITION(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

MP_POSITION::MP_POSITION(float x, float y)
{
	this->x = x;
	this->y = y;
	z = 0.f;
}

MP_POSITION MP_POSITION::operator+(const MP_POSITION& pos)
{
	MP_POSITION temp(x, y, z);
	temp.x += pos.x;
	temp.y += pos.y;
	temp.z += pos.z;
	return temp;
}

MP_POSITION MP_POSITION::operator+=(const MP_POSITION& pos)
{
	*this = *this + pos;
	return *this;
}

MP_POSITION MP_POSITION::operator-(const MP_POSITION& pos)
{
	MP_POSITION temp(x, y, z);
	temp.x -= pos.x;
	temp.y -= pos.y;
	temp.z -= pos.z;
	return temp;
}

MP_POSITION MP_POSITION::operator-=(const MP_POSITION& pos)
{
	*this = *this - pos;
	return *this;
}

float MP_POSITION::Length()
{
	float d = sqrtf(x*x + y*y + z*z);
	return d;
}

void MP_POSITION::Normalize()
{
	float d = Length();
	x /= d;
	y /= d;
	z /= d;
}

// rus: Векторное произведение
void MP_POSITION::Cross(const MP_POSITION* pV1, const MP_POSITION* pV2)
{
	x = pV1->y*pV2->z - pV1->z*pV2->y;
	y = pV1->z*pV2->x - pV1->x*pV2->z;
	z = pV1->x*pV2->y - pV1->y*pV2->x;
}

// rus: Скалярное произведение
float MP_POSITION::Dot(const MP_POSITION* p)
{
	return x*p->x + y*p->y + z*p->z;
}

// ------------------------------------------------------------------------------------------

TEXTURE_TYPE MP_Manager::mTextureType = TT_PNG;
//TEXTURE_TYPE MP_Manager::mTextureType = TT_SWI;
txMP_Device* MP_Manager::device = NULL;

// eng: Class that is used as storage for Magic Particles emitters
// rus: Класс, который является хранилищем эмиттеров Magic Particles
MP_Manager::MP_Manager()
{
	MP_Device::MP = this;
	MP_Atlas::MP = this;
	MP_Copy::MP = this;
	MP_Platform::MP = this;

	platform = NULL;

	k_emitter = 0;
	max_emitter = 10;
	m_emitter = TRACE_NEW_ARRAY(MP_Emitter*, max_emitter, m_emitter);
	m_descriptor = TRACE_NEW_ARRAY(int, max_emitter, m_descriptor);
	for (int i = 0; i < max_emitter; ++i)
	{
		m_emitter[i] = NULL;
		m_descriptor[i] = 0;
	}

	k_atlas = 0;
	m_atlas = NULL;

	k_copy = 0;
	m_copy = NULL;

	k_ptc = 0;
	m_ptc = NULL;

	atlas_width = atlas_height = 1024;
	atlas_frame_step = 1;
	atlas_scale_step = 0.01f;

	next_descriptor = 0;
	next_index = -1;

	is_new_atlas = false;
}

// eng: Cleaning up
// rus: Очистка
void MP_Manager::Destroy()
{
	int i;
	for (i = 0; i < max_emitter; ++i)
	{
		TRACE_DELETE(m_emitter[i]);
	}

	TRACE_DELETE_ARRAY(m_emitter);
	TRACE_DELETE_ARRAY(m_descriptor);

	k_emitter = 0;
	max_emitter = 0;

	for (i = 0; i < k_atlas; ++i)
	{
		TRACE_DELETE(m_atlas[i]);
	}
	TRACE_DELETE_ARRAY(m_atlas);

	k_atlas = 0;

	for (i = 0; i < k_copy; ++i)
	{
		TRACE_DELETE(m_copy[i]);
	}
	TRACE_DELETE_ARRAY(m_copy);

	k_copy = 0;

	for (i = 0; i < k_ptc; ++i)
	{
		TRACE_DELETE(m_ptc[i]);
	}
	TRACE_DELETE_ARRAY(m_ptc);

	k_ptc = 0;

	MP_Device::MP = NULL;
	MP_Atlas::MP = NULL;
	MP_Copy::MP = NULL;

	TRACE_DELETE(platform);

	MP_Platform::MP = NULL;
}

// eng: Initialization
// rus: Инициализация
void MP_Manager::Initialization(int axis, MP_Platform* platform, int atlas_width, int atlas_height, int atlas_frame_step, float atlas_starting_scale, float atlas_scale_step, bool copy_mode)
{
	this->platform = platform;

	Magic_SetAxis(axis);

	Magic_SetCleverModeForAtlas(true);
	Magic_SetStartingScaleForAtlas(atlas_starting_scale);

	this->atlas_width = atlas_width;
	this->atlas_height = atlas_height;
	this->atlas_frame_step = atlas_frame_step;
	this->atlas_scale_step = atlas_scale_step;

	this->copy_mode = copy_mode;
}

// eng: Returning descriptor of first emitter. 
// rus: Получение дескриптора первого эмиттера
HM_EMITTER MP_Manager::GetFirstEmitter()
{
	next_descriptor = 0;
	next_index = -1;

	if (k_emitter)
	{
		next_descriptor = m_descriptor[0];
		next_index = 0;
	}

	return next_descriptor;
}

/// eng: Returning descriptor of next emitter. 
// rus: Получение дескриптора следующего эмиттера
HM_EMITTER MP_Manager::GetNextEmitter(HM_EMITTER hmEmitter)
{
	if (next_index == -1 || hmEmitter != next_descriptor)
	{
		next_index = -1;
		for (int i = 0; i < k_emitter; ++i)
		{
			if (m_descriptor[i] == hmEmitter)
			{
				next_index = i;
				break;
			}
		}
	}

	next_descriptor = 0;

	if (next_index != -1)
	{
		++next_index;
		if (next_index < k_emitter)
		{
			next_descriptor = m_descriptor[next_index];
		}
		else
		{
			next_index = -1;
		}
	}
	return next_descriptor;
}

// eng: Returning the emitter by its descriptor
// rus: Возвращение эмиттера по дескриптору
MP_Emitter* MP_Manager::GetEmitter(HM_EMITTER hmEmitter)
{
	if (txMath::isInRange(hmEmitter, 0, max_emitter - 1))
	{
		return m_emitter[hmEmitter];
	}
	return NULL;
}

// eng: Returning the emitter by name
// rus: Возвращание эмиттера по имени
MP_Emitter* MP_Manager::GetEmitterByName(const char* name)
{
	HM_EMITTER hmEmitter = GetFirstEmitter();
	while (hmEmitter != NULL)
	{
		MP_Emitter* emitter = GetEmitter(hmEmitter);
		const char* emitter_name = emitter->GetEmitterName();
		if (!strcmp(name, emitter_name))
		{
			// eng: name coincides
			// rus: имя совпадает
			return emitter;
		}
		hmEmitter = GetNextEmitter(hmEmitter);
	}

	return NULL;
}

// eng: Loading all emitters from emitters folder
// rus: Загрузка всех эмиттеров из всех файлов
void MP_Manager::LoadAllEmitters(txMap<std::string, PtcStruct>& particleFileList)
{
	// 此处暂时用固定的路径来测试
	std::pair<std::string, PtcStruct> filePair;
	LoadEmittersFromFile("example3d.ptc", NULL, NULL, filePair);
	particleFileList.insert(filePair.first, filePair.second);
	//	const char* file=platform->GetFirstFile();
	//	while (file)
	//	{
	//		LoadEmittersFromFile(file);
	//		file=platform->GetNextFile();
	//	}
	//	RefreshAtlas();
}

// eng: Loading all the emitters and animated folders from the file specified
// rus: Загрузка всех эмиттеров из указанного файла. Загружаются эмиттеры и анимированные папки
bool MP_Manager::LoadEmittersFromFile(const char* file, txNode* parent, HM_FILE* pFileHandle, std::pair<std::string, PtcStruct>& filePair)
{
	filePair.first = file;
	std::string ptc_file = GetPathToPTC();
	ptc_file += file;

	HM_FILE mf = OpenPTC(ptc_file.c_str());
	if (pFileHandle != NULL)
	{
		*pFileHandle = mf;
	}
	if (mf <= 0)
	{
		ENGINE_ERROR("error : can not open file %s", file);
	}
	else if (mf > 0)
	{
		// eng: file was opened
		return LoadFolder(mf, "", parent, filePair);
	}
	return false;
}

HM_FILE MP_Manager::LoadEmittersFromFileInMemory(char* address, txNode* parent, std::pair<std::string, PtcStruct>& filePair)
{
	HM_STREAM hmStream = Magic_StreamOpenMemory(address, 0, MAGIC_STREAM_READ);
	HM_FILE hmFile = Magic_OpenStream(hmStream);
	if (hmFile > 0)
	{
		// eng: file was opened
		// rus: файл успешно открыт
		MP_PTC* ptc = TRACE_NEW(MP_PTC, ptc);
		ptc->hmFile = hmFile;
		ptc->hmStream = hmStream;
		ptc->data = address;
		AddPTC(ptc);

		LoadFolder(hmFile, "", parent, filePair);
	}
	else
	{
		Magic_StreamClose(hmStream);
		char* p = const_cast<char*>(address);
		TRACE_DELETE_ARRAY(p);
	}
	return hmFile;
}

// eng: Closing file
// rus: Выгрузка одного файла
int MP_Manager::CloseFile(HM_FILE hmFile)
{
	RefreshAtlas();
	return DeletePTC(hmFile);
}

// eng: Closing all files
// rus: Выгрузка всех файлов
void MP_Manager::CloseFiles()
{
	RefreshAtlas();
	if (k_ptc)
	{
		int i = 0;
		do
		{
			TRACE_DELETE(m_ptc[i]);
			++i;
		} while (i < k_ptc);
		TRACE_DELETE_ARRAY(m_ptc);
		k_ptc = 0;
	}
}

// eng: Duplicating specified emitter
// rus: Дублирование указанного эмиттера
HM_EMITTER MP_Manager::DuplicateEmitter(HM_EMITTER hmEmitter)
{
	MP_Emitter* from = GetEmitter(hmEmitter);
	if (from)
	{
		if (Magic_IsInterval1(hmEmitter) && (!from->copy))
		{
			// eng: it is necessary firstly to create particles copy
			// rus: необходимо сначала создать копию частиц
			from->Restart();
		}

		MP_Emitter* emitter = TRACE_NEW(MP_Emitter, emitter, 0, this);
		*emitter = *from;
		AddEmitter(emitter);
		return emitter->GetEmitter();
	}
	return 0;
}

// eng: Updating emitters taking into account the passed time
// rus: Обновление эмиттеров по таймеру
int MP_Manager::UpdateByTimer()
{
	static int fps = 0;
	static int fps_counter = 0;
	static unsigned long old_time = 0;

	static unsigned long last_time = 0;
	double rate = 0.01;
	unsigned long new_time = GetTick();
	if (new_time > last_time)
	{
		rate = new_time - last_time;
		last_time = new_time;
		if (rate > 500)
			rate = 0.01;
	}

	unsigned long rtime = new_time - old_time;
	if (rtime > 1000)
	{
		// eng: more than 1 second passed
		// rus: прошло больше секунды
		double percent = ((double)rtime) / 10.f;
		fps = (int)(percent*fps_counter / 100);
		fps_counter = 0;
		old_time = new_time;
	}

	++fps_counter;

	Update(rate);

	return fps;
}

// eng: Updating emitters
// rus: Обновление эмиттеров
void MP_Manager::Update(double time)
{
	HM_EMITTER hmEmitter = GetFirstEmitter();

	while (hmEmitter)
	{
		MP_Emitter* emitter = GetEmitter(hmEmitter);
		int EMITTER_STATE = emitter->GetState();
		if (EMITTER_STATE == ES_UPDATE || EMITTER_STATE == ES_INTERRUPT)
		{
			emitter->Update((float)time);

			MAGIC_EVENT evt;
			while (Magic_GetNextEvent(&evt) == MAGIC_SUCCESS)
			{
				function_event[evt.event](&evt);	// обработка события
			}
		}

		hmEmitter = GetNextEmitter(hmEmitter);
	}
}

// eng: Rendering all emitters
// rus: Рисование эмиттеров
int MP_Manager::Render(txCamera* camera)
{
	RefreshAtlas();
	device->BeginScene(camera);

	// eng: visualisation of all emitters
	// rus: визуализация всех эмиттеров
	int k_particles = 0;
	HM_EMITTER hmEmitter = GetFirstEmitter();
	while (hmEmitter)
	{
		MP_Emitter* emitter = GetEmitter(hmEmitter);
		k_particles += emitter->Render();
		hmEmitter = GetNextEmitter(hmEmitter);
	}
	device->EndScene();
	return k_particles;
}

// eng: Stopping all the emitters
// rus: Остановка эмиттеров
void MP_Manager::Stop()
{
	HM_EMITTER hmEmitter = GetFirstEmitter();

	while (hmEmitter)
	{
		MP_Emitter* emitter = GetEmitter(hmEmitter);
		emitter->SetState(ES_STOP);
		hmEmitter = GetNextEmitter(hmEmitter);
	}
}

// eng: Loading folder
// rus: Загрузка папки
bool MP_Manager::LoadFolder(HM_FILE file, const char* path, txNode* parent, std::pair<std::string, PtcStruct>& filePair)
{
	Magic_SetCurrentFolder(file, path);

	MAGIC_FIND_DATA find;
	const char* name = Magic_FindFirst(file, &find, MAGIC_FOLDER | MAGIC_EMITTER);
	while (name)
	{
		if (find.animate)
		{
			MP_Emitter* pEmitter = LoadEmitter(file, name, parent);
			if (pEmitter == NULL)
			{
				ENGINE_ERROR("error : failed to load emitter %s", name);
			}
			else
			{
				filePair.second.mEmitterList.insert(name, pEmitter->GetEmitter());
			}
		}
		else
		{
			LoadFolder(file, name, parent, filePair);
		}
		name = Magic_FindNext(file, &find);
	}
	Magic_SetCurrentFolder(file, "..");
	return filePair.second.mEmitterList.size() > 0;
}

// eng: Loading emitter
// rus: Загрузка конкретного эмиттера
MP_Emitter* MP_Manager::LoadEmitter(HM_FILE file, const char* path, txNode* parent)
{
	// eng: it is necessary to load emitter from file
	// rus: нужно извлечь эмиттер из файла
	MP_Emitter* em = NULL;
	HM_EMITTER emitter = Magic_LoadEmitter(file, path);
	if (emitter)
	{
		em = TRACE_NEW(MP_Emitter, em, emitter, this);
		em->SetParentNode(parent);
		const char* ptc = Magic_GetFileName(file);
		if (ptc && Magic_HasTextures(file))
			em->restore_file = ptc;

		AddEmitter(em);
		if (Magic_GetStaticAtlasCount(file))
			em->is_atlas = true;
		else
			is_new_atlas = true;
	}
	return em;
}

// eng: Adding new emitter into array
// rus: Добавление нового эмиттера в массив
void MP_Manager::AddEmitter(MP_Emitter* emitter)
{
	int i;

	next_descriptor = 0;
	next_index = -1;

	int index = (int)emitter->GetEmitter();

	while (index >= max_emitter)
	{
		int new_max_emitter = max_emitter + 10;

		MP_Emitter** vm_emitter = TRACE_NEW_ARRAY(MP_Emitter*, new_max_emitter, vm_emitter);
		for (i = 0; i < max_emitter; ++i)
		{
			vm_emitter[i] = m_emitter[i];
		}
		TRACE_DELETE_ARRAY(m_emitter);
		m_emitter = vm_emitter;

		int* vm_descriptor = TRACE_NEW_ARRAY(int, new_max_emitter, vm_descriptor);
		for (i = 0; i < max_emitter; ++i)
		{
			vm_descriptor[i] = m_descriptor[i];
		}
		TRACE_DELETE_ARRAY(m_descriptor);
		m_descriptor = vm_descriptor;

		for (i = max_emitter; i < new_max_emitter; ++i)
		{
			m_emitter[i] = NULL;
			m_descriptor[i] = 0;
		}

		max_emitter = new_max_emitter;
	}

	m_emitter[index] = emitter;
	m_descriptor[k_emitter] = index;
	++k_emitter;
}

// eng: Refreshing textural atlases
// rus: Построение текстурного атласа
void MP_Manager::RefreshAtlas()
{
	int i;

	if (is_new_atlas)
	{
		// eng: new emitters were added, it is necessary to create new atlases for them
		// rus: были добавлены новые эмиттеры, необходимо создать для них атласы
		is_new_atlas = false;

		int k = GetEmitterCount();
		if (k > 0)
		{
			HM_EMITTER* hm_emitter = TRACE_NEW_ARRAY(HM_EMITTER, k, hm_emitter);

			k = 0;

			HM_EMITTER hmEmitter = GetFirstEmitter();
			while (hmEmitter != NULL)
			{
				MP_Emitter* emitter = GetEmitter(hmEmitter);
				if (!emitter->is_atlas)
				{
					emitter->is_atlas = true;
					hm_emitter[k] = hmEmitter;
					++k;
				}
				hmEmitter = GetNextEmitter(hmEmitter);
			}

			if (k > 0)
			{
				Magic_CreateAtlasesForEmitters(atlas_width, atlas_height, k, hm_emitter, atlas_frame_step, atlas_scale_step);
			}
			TRACE_DELETE_ARRAY(hm_emitter);
		}
	}

	MAGIC_CHANGE_ATLAS c;
	while (Magic_GetNextAtlasChange(&c) == MAGIC_SUCCESS)
	{
		switch (c.type)
		{
		case MAGIC_CHANGE_ATLAS_LOAD:
			// eng: loading of frame in atlas
			// rus: загрузка кадра в атлас
			m_atlas[c.index]->LoadTexture(&c);
			break;

		case MAGIC_CHANGE_ATLAS_CLEAN:
			// eng: cleaning up of rectangle in atlas
			// rus: очистка прямоугольника в атласе
			m_atlas[c.index]->CleanRectangle(&c);
			break;

		case MAGIC_CHANGE_ATLAS_CREATE:
			// eng: creating of atlas
			// rus: создание атласа
			if (m_atlas)
			{
				// eng: broadening of atlas array
				// rus: расширение массив атласов
				MP_Atlas** vm_atlas = TRACE_NEW_ARRAY(MP_Atlas*, k_atlas + 1, vm_atlas);
				for (i = 0; i < k_atlas; ++i)
					vm_atlas[i] = m_atlas[i];
				TRACE_DELETE_ARRAY(m_atlas);
				m_atlas = vm_atlas;
			}
			else
			{
				m_atlas = TRACE_NEW_ARRAY(MP_Atlas*, 1, m_atlas);
			}

			m_atlas[k_atlas] = device->NewAtlas(c.width, c.height, c.file);
			++k_atlas;

			break;

		case MAGIC_CHANGE_ATLAS_DELETE:
			// eng: Deleting of atlas
			// rus: удаление атласа
			TRACE_DELETE(m_atlas[c.index]);

			if (k_atlas == 1)
			{
				TRACE_DELETE_ARRAY(m_atlas);
			}
			else
			{
				MP_Atlas** vm_atlas = TRACE_NEW_ARRAY(MP_Atlas*, k_atlas - 1, vm_atlas);
				for (i = 0; i < c.index; ++i)
					vm_atlas[i] = m_atlas[i];
				for (i = c.index + 1; i < k_atlas; ++i)
					vm_atlas[i - 1] = m_atlas[i];
				TRACE_DELETE_ARRAY(m_atlas);
				m_atlas = vm_atlas;
			}
			--k_atlas;
		}
	}
}

// eng: Restoring textural atlas in cases of loosing textures
// rus: Восстановление текстурного атласа в случае потери текстур
void MP_Manager::RestoreAtlas()
{
	if (k_emitter)
	{
		int i;

		// eng: Recreating of static atlas
		// rus: Перестраиваем статические атласы
		for (i = 0; i < k_atlas; ++i)
		{
			MP_Atlas* atlas = m_atlas[i];
			std::string file = atlas->GetFile();
			if (!(file.empty()))
			{
				// eng: it is necessary to reload texture
				// rus: необходимо перегрузить текстуру
				int width, height;
				atlas->GetSize(width, height);

				TRACE_DELETE(atlas);
				m_atlas[i] = device->NewAtlas(width, height, file.c_str());
			}
		}

		// eng: Recreating of dynamic atlas
		// rus: Перестраиваем динамические атласы
		int k_restore_file = 0;
		std::string** m_restore_file = TRACE_NEW_ARRAY(std::string*, k_emitter, m_restore_file);

		HM_EMITTER hmEmitter = GetFirstEmitter();
		while (hmEmitter != NULL)
		{
			MP_Emitter* emitter = GetEmitter(hmEmitter);

			bool add = true;

			if (emitter->restore_file.empty())
			{
				add = false;
			}
			else
			{
				for (int j = 0; j < k_restore_file; ++j)
				{
					if (*(m_restore_file[j]) == emitter->restore_file)
					{
						add = false;
						break;
					}
				}
			}

			if (add)
			{
				m_restore_file[k_restore_file] = &(emitter->restore_file);
				++k_restore_file;
			}

			hmEmitter = GetNextEmitter(hmEmitter);
		}

		HM_FILE* m_opened_file = NULL;
		if (k_restore_file)
		{
			// rus: loading all found files with textures
			// rus: загружаем все отобранные файлы с текстурами
			m_opened_file = TRACE_NEW_ARRAY(HM_FILE, k_restore_file, m_opened_file);
			for (i = 0; i < k_restore_file; ++i)
			{
				std::string path = GetPathToPTC();
				path += *(m_restore_file[i]);
				m_opened_file[i] = OpenPTC(path.c_str());
			}
		}

		// eng: Recreating of dynamic atlas
		// rus: Перестраиваем динамические атласы
		Magic_CreateAtlases(atlas_width, atlas_height, atlas_frame_step, atlas_scale_step);

		RefreshAtlas();

		if (k_restore_file)
		{
			// eng: Unload ptc-file with textures
			// rus: выгружаем ptc-файлы с текстурами
			for (i = 0; i < k_restore_file; ++i)
				CloseFile(m_opened_file[i]);

			TRACE_DELETE_ARRAY(m_opened_file);
		}
		TRACE_DELETE_ARRAY(m_restore_file);
		k_restore_file = 0;
	}
}

// eng: Deleting specified emitter 
// rus: Удаление указанного эмиттера
int MP_Manager::DeleteEmitter(HM_EMITTER hmEmitter)
{
	int result = MAGIC_ERROR;
	next_descriptor = 0;
	next_index = -1;

	for (int j = 0; j < k_emitter; ++j)
	{
		HM_EMITTER hme = m_descriptor[j];
		if (hme == hmEmitter)
		{
			// it is necessary to delete this element from index array
			// нужно удалить данный элемент из индексного массива
			for (int k = j + 1; k < k_emitter; ++k)
			{
				m_descriptor[k - 1] = m_descriptor[k];
			}

			--k_emitter;

			m_descriptor[k_emitter] = 0;
			TRACE_DELETE(m_emitter[hmEmitter]);

			result = MAGIC_SUCCESS;

			break;
		}
	}

	return result;
}

// eng: Adding file with particles copy
// rus: Добавление файла с копией частиц
MP_Copy* MP_Manager::AddCopy(MP_Emitter* emitter)
{
	if (m_copy)
	{
		MP_Copy** vm_copy = TRACE_NEW_ARRAY(MP_Copy*, k_copy + 1, vm_copy);
		for (int i = 0; i < k_copy; ++i)
		{
			vm_copy[i] = m_copy[i];
		}
		TRACE_DELETE_ARRAY(m_copy);
		m_copy = vm_copy;
	}
	else
	{
		m_copy = TRACE_NEW_ARRAY(MP_Copy*, 1, m_copy);
	}

	MP_Copy* copy = TRACE_NEW(MP_Copy, copy, emitter);
	m_copy[k_copy] = copy;
	++k_copy;

	return copy;
}

// eng: Deleting file with particles copy
// rus: Удаление файла с копией частиц
void MP_Manager::DeleteCopy(MP_Copy* copy)
{
	// eng: it is necessary to delete copy
	// rus: надо удалить копию
	int i;

	int index = -1;
	for (i = 0; i < k_copy; ++i)
	{
		if (m_copy[i] == copy)
		{
			index = i;
			break;
		}
	}
	TRACE_DELETE(m_copy[index]);

	if (k_copy == 1)
	{
		TRACE_DELETE_ARRAY(m_copy);
	}
	else
	{
		MP_Copy** vm_copy = TRACE_NEW_ARRAY(MP_Copy*, k_copy - 1, vm_copy);
		for (i = 0; i < index; ++i)
		{
			vm_copy[i] = m_copy[i];
		}
		for (i = index + 1; i < k_copy; ++i)
		{
			vm_copy[i - 1] = m_copy[i];
		}
		TRACE_DELETE_ARRAY(m_copy);
		m_copy = vm_copy;
	}

	--k_copy;
}

// eng: Searching among files containing particle copies by specified emitter id
// rus: Поиск среди файлов копий частиц соответствующего указанному идентификатору эмиттера
MP_Copy* MP_Manager::FindCopy(unsigned int emitter_id)
{
	if (GetCopyMode())
	{
		for (int i = 0; i < k_copy; ++i)
		{
			MP_Copy* copy = m_copy[i];
			if (copy->GetEmitterID() == emitter_id)
			{
				return copy;
			}
		}
	}
	return NULL;
}

// eng: Adding open file
// rus: Добавление открытого файла
void MP_Manager::AddPTC(MP_PTC* ptc_file)
{
	if (ptc_file->data == NULL)
	{
		ENGINE_ERROR("error : ptc data is NULL");
		return;
	}
	if (m_ptc)
	{
		MP_PTC** vm_ptc = TRACE_NEW_ARRAY(MP_PTC*, k_ptc + 1, vm_ptc);
		for (int i = 0; i < k_ptc; ++i)
		{
			vm_ptc[i] = m_ptc[i];
		}
		TRACE_DELETE_ARRAY(m_ptc);
		m_ptc = vm_ptc;
	}
	else
	{
		m_ptc = TRACE_NEW_ARRAY(MP_PTC*, 1, m_ptc);
	}
	m_ptc[k_ptc] = ptc_file;
	++k_ptc;
}

// eng: Deleting open file
// rus: Удаление открытого файла
int MP_Manager::DeletePTC(HM_FILE hmFile)
{
	int result = MAGIC_ERROR;
	for (int i = 0; i < k_ptc; ++i)
	{
		if (m_ptc[i]->hmFile == hmFile)
		{
			result = m_ptc[i]->Close();
			TRACE_DELETE(m_ptc[i]);

			if (k_ptc == 1)
			{
				TRACE_DELETE_ARRAY(m_ptc);
			}
			else
			{
				MP_PTC** vm_ptc = TRACE_NEW_ARRAY(MP_PTC*, k_ptc - 1, vm_ptc);
				int j;
				for (j = 0; j < i; ++j)
				{
					vm_ptc[j] = m_ptc[j];
				}
				for (j = i + 1; j < k_ptc; ++j)
				{
					vm_ptc[j - 1] = m_ptc[j];
				}
				TRACE_DELETE_ARRAY(m_ptc);
				m_ptc = vm_ptc;
			}

			--k_ptc;

			break;
		}
	}
	return result;
}

// ------------------------------------------------------------------------------------------------

// eng: Class, specialized for work with the emitters
// rus: Класс, который хранит загруженные эмиттеры

txMP_Device* MP_Emitter::device = NULL;

MP_Emitter::MP_Emitter(HM_EMITTER emitter, MP_Manager* owner)
:
txObject(OT_EMITTER)
{
	this->emitter = emitter;
	this->owner = owner;
	z = 0.f;
	first_restart = true;
	copy = NULL;
	restore_file = EMPTY_STRING;
	mState = ES_STOP;
	mUpdateMode = EUM_LOOP;
	is_atlas = false;
	mLasSettedtRotation.x = 0.0f;
	mLasSettedtRotation.y = 0.0f;
	mLasSettedtRotation.z = 0.0f;
	mLasSettedtRotation.w = 1.0f;
	mLastSettedScale = 1.0f;
	mParent = NULL;
	mMagicChangeMode = MCM_EMITTER_AND_PARTICLES;
	Magic_SetEmitterPositionMode(emitter, true);
	Magic_SetEmitterDirectionMode(emitter, true);
}

MP_Emitter::~MP_Emitter()
{
	if (copy)
	{
		copy->DecReference();
		copy = NULL;
	}

	if (mParent != NULL)
	{
		mNodeManager->DestroyNode(mParent->GetName());
		mParent = NULL;
	}

	Magic_UnloadEmitter(emitter);
}

bool MP_Emitter::GetInterpolation()
{
	return Magic_IsInterpolationMode(emitter);
}

void MP_Emitter::SetInterpolation(bool value)
{
	Magic_SetInterpolationMode(emitter, value);
}

// 注意,此函数不能在节点的更新函数中调用,否则会引起迭代器失效
void MP_Emitter::SetParentNode(txNode* parent)
{
	// 如果发射器节点为空,则创建发射器节点
	if (mParent == NULL)
	{
		mParent = mNodeManager->CreateNode();
		mParent->AttachObject(this);
	}
	// 如果发射器节点已经创建,并且有父节点,则先从父节点上取下,再挂接到新的父节点上
	else
	{
		if (mParent->GetParent() != NULL)
		{
			mParent->GetParent()->RemoveChildNode(mParent);
		}
	}
	if (parent != NULL)
	{
		parent->AddChildNode(mParent);
	}
}

MP_Emitter& MP_Emitter::operator=(const MP_Emitter& from)
{
	if (copy)
	{
		copy->DecReference();
		copy = NULL;
	}

	mState = from.mState;

	is_atlas = from.is_atlas;

	z = from.z;

	if (from.copy)
	{
		copy = from.copy;
		copy->IncReference(this);
	}

	restore_file = from.restore_file;

	emitter = Magic_DuplicateEmitter(from.emitter);

	return *this;
}

// eng: Returning the name of the emitter
// rus: Возвращение имени эмиттера
const char* MP_Emitter::GetEmitterName()
{
	return Magic_GetEmitterName(emitter);
}

// eng: Restarting of emitter
// rus: Установка эмиттера на стартовую позицию
void MP_Emitter::Restart()
{
	if (Magic_IsInterval1(emitter))
	{
		// eng: animation starts not from beginning
		// rus: анимация начинается не с начала
		if (!copy)
		{
			copy = owner->FindCopy(Magic_GetEmitterID(emitter));
			if (!copy)
				copy = owner->AddCopy(this);
		}

		copy->LoadParticles(this);
	}
	else
	{
		Magic_Restart(emitter);
	}

	first_restart = false;
}

// eng: Position of emitter
// rus: Позиция эмиттера
void MP_Emitter::GetPosition(MP_POSITION& position)
{
	MAGIC_POSITION pos;
	Magic_GetEmitterPosition(emitter, &pos);
	position.x = pos.x;
	position.y = pos.y;
#ifdef MAGIC_3D
	position.z = pos.z;
#else
	position.z=z;
#endif
}

void MP_Emitter::SetPosition(MP_POSITION& position)
{
	// 计算与上一次设置位置的偏移
	MP_POSITION diffPos = position - mLastSettedPosition;
	// 然后加上该偏移位置
	MP_POSITION oldPos;
	GetPosition(oldPos);

	MAGIC_POSITION newPos;

	newPos.x = oldPos.x + diffPos.x;
	newPos.y = oldPos.y + diffPos.y;

#ifdef MAGIC_3D
	newPos.z = oldPos.z + diffPos.z;
#else
	z=position.z;
#endif
	Magic_SetEmitterPosition(emitter, &newPos);
	mLastSettedPosition = position;
}

// eng: Moving the emitter to the position specified allowing restart. 
// rus: Перемещение эмиттера в указанную позицию с возможностью перезапуска. При перемещении все существующие частицы перемещаются вместе с эмиттером
void MP_Emitter::Move(MP_POSITION& position, const bool& restart)
{
	if (restart)
	{
		Restart();
		SetPosition(position);
	}
	else
	{
		bool mode = Magic_GetEmitterPositionMode(emitter);
		if (mode == MCM_EMITTER_ONLY)
		{
			// eng: temporary setting mode of movement together with particles
			// rus: временно устанавливаем режим перемещения вместе с частицами
			Magic_SetEmitterPositionMode(emitter, MCM_EMITTER_AND_PARTICLES);
		}

		SetPosition(position);

		if (mode == MCM_EMITTER_ONLY)
		{
			// eng: restore previous mode of movement
			// rus: возвращаем на место старый режим перемещения
			Magic_SetEmitterPositionMode(emitter, MCM_EMITTER_ONLY);
		}
	}
}

// eng: Offsetting the current emitter position by the value specified
// rus: Смещение текущей позиции эмиттера на указанную величину. Будет использован текущий режим перемещения эмиттера
void MP_Emitter::Offset(MP_POSITION& offset)
{
	MP_POSITION pos;
	GetPosition(pos);
	pos += offset;
	SetPosition(pos);
}

// eng: Direction of emitter
// rus: Направление эмиттера
void MP_Emitter::SetDirection(MAGIC_DIRECTION* direction)
{
	// 先将四元数单位化
	float len = std::sqrt(direction->x * direction->x + direction->y * direction->y + direction->z * direction->z + direction->w * direction->w);
	direction->x /= len;
	direction->y /= len;
	direction->z /= len;
	direction->w /= len;

	// 要设置的旋转值
	QUATERNION setQua;
	setQua.x = direction->x;
	setQua.y = direction->y;
	setQua.z = direction->z;
	setQua.w = direction->w;

	// 当前的旋转值
	MAGIC_DIRECTION curDir;
	GetDirection(&curDir);
	QUATERNION curQua;
	curQua.x = curDir.x;
	curQua.y = curDir.y;
	curQua.z = curDir.z;
	curQua.w = curDir.w;

	// 上一次设置的旋转值
	QUATERNION lastQua;
	lastQua.x = mLasSettedtRotation.x;
	lastQua.y = mLasSettedtRotation.y;
	lastQua.z = mLasSettedtRotation.z;
	lastQua.w = mLasSettedtRotation.w;

	// 将上一次设置的旋转值还原,在设置新的旋转
	QUATERNION lastInverseQuat = txMath::inverse(lastQua);
	QUATERNION newQua = setQua * lastInverseQuat * curQua;
	MAGIC_DIRECTION newDir;
	newDir.x = newQua.x;
	newDir.y = newQua.y;
	newDir.z = newQua.z;
	newDir.w = newQua.w;
	// 单位化
	len = std::sqrt(newDir.x * newDir.x + newDir.y * newDir.y + newDir.z * newDir.z + newDir.w * newDir.w);
	newDir.x /= len;
	newDir.y /= len;
	newDir.z /= len;
	newDir.w /= len;
	Magic_SetEmitterDirection(emitter, &newDir);
	mLasSettedtRotation = *direction;
	//Magic_SetEmitterDirection(emitter, direction);
}

void MP_Emitter::GetDirection(MAGIC_DIRECTION* direction)
{
	Magic_GetEmitterDirection(emitter, direction);
}

// eng: Setting the emitter direction to the specified value with the restart ability
// rus: Поворот эмиттера в указанное направление с возможностью перезапуска. При повороте все существующие частицы поворачиваются вместе с эмиттером
void MP_Emitter::Direct(MAGIC_DIRECTION* direction, const bool& restart)
{
	if (restart)
	{
		Restart();
		SetDirection(direction);
	}
	else
	{
		bool mode = Magic_GetEmitterDirectionMode(emitter);
		if (mode == MCM_EMITTER_ONLY)
		{
			// eng: temporary setting mode of movement together with particles
			// rus: временно устанавливаем режим перемещения вместе с частицами
			Magic_SetEmitterDirectionMode(emitter, MCM_EMITTER_AND_PARTICLES);
		}

		SetDirection(direction);

		if (mode == MCM_EMITTER_ONLY)
		{
			// eng: restore previous mode of movement
			// rus: возвращаем на место старый режим перемещения
			Magic_SetEmitterDirectionMode(emitter, MCM_EMITTER_ONLY);
		}
	}
}

// eng: Rotating of the emitter by the specified value
// rus: Вращение эмиттера на указанную величину. Будет использован текущий режим вращения эмиттера
void MP_Emitter::Rotate(MAGIC_DIRECTION* offset)
{
	MAGIC_DIRECTION q;
	GetDirection(&q);
#ifdef MAGIC_3D
	MagicQuaternionInversion(&q);
	MagicQuaternionMul(&q, &q, offset);
#else
	q.angle+=offset->angle;
#endif
	SetDirection(&q);
}

// eng: Setting the scale of the emitter
// rus: Установка масштаба эмиттера
void MP_Emitter::SetScale(const float& scale)
{
	// 先将上一次的缩放值还原,再设置新的缩放
	float curScale = GetScale() / mLastSettedScale * scale;
	Magic_SetScale(emitter, curScale);
	mLastSettedScale = scale;
}

float MP_Emitter::GetScale()
{
	return Magic_GetScale(emitter);
}

// eng: Setting the state of the emitter
// rus: Установка статуса эмиттера
void MP_Emitter::SetState(const EMITTER_STATE& state)
{
	if (mState != state)
	{
		if (state == ES_UPDATE && Magic_IsInterrupt(emitter))
		{
			// eng: it is necessary to turn off interrupting of emitter work
			// rus: необходимо отключить прерывание работы эмиттера
			Magic_SetInterrupt(emitter, false);
		}

		if (state == ES_STOP && mState != ES_INTERRUPT)
		{
			// eng: unload particles from memory
			// rus: выгружаем пространство частиц из памяти
			Magic_Stop(emitter);
		}
		else
		{
			if (state == ES_UPDATE || state == ES_INTERRUPT)
			{
				// eng: start emitter
				// rus: стартуем эмиттер
				if (!first_restart)
				{
					if (mState == ES_STOP || (!Magic_InInterval(emitter)))
					{
						// rus: позиция анимации эмиттера не входит в интервал видимости, необходимо осуществить установку на начало
						if (copy)
							copy->LoadParticles(this);
					}
				}

				if (state == ES_INTERRUPT)
					Magic_SetInterrupt(emitter, true);
			}
		}

		mState = state;
	}
}

void MP_Emitter::SetMagicChangeMode(const MAGIC_CHANGE_MODE& mode)
{
	mMagicChangeMode = mode;
	switch (mMagicChangeMode)
	{
	case MCM_EMITTER_ONLY:
		Magic_SetEmitterPositionMode(emitter, false);
		Magic_SetEmitterDirectionMode(emitter, false);
		break;

	case MCM_EMITTER_AND_PARTICLES:
		Magic_SetEmitterPositionMode(emitter, true);
		Magic_SetEmitterDirectionMode(emitter, true);
		break;
	}
}

// eng: Updating emitter
void MP_Emitter::Update(float fElaspedTime)
{
	if (mState == ES_UPDATE || mState == ES_INTERRUPT)
	{
		// 传进来的时间是以秒为单位的,需要转换为毫秒
		fElaspedTime *= 1000;
		if (first_restart)
			Restart();

		if (mParent != NULL)
		{
			// 位置
			const VECTOR3& pos = mParent->GetWorldPosition();
			MP_POSITION mpPos(pos.x, pos.y, -pos.z);
			SetPosition(mpPos);

			// 方向
			QUATERNION qRote = txMath::matrix3ToQuat(mParent->GetWorldRotationMatrix());
			txMath::normalize(qRote);
			MAGIC_DIRECTION mpDir;
			mpDir.x = qRote.x;
			mpDir.y = qRote.y;
			mpDir.z = qRote.z;
			mpDir.w = qRote.w;
			SetDirection(&mpDir);

			// 缩放
			const VECTOR3& scale = mParent->GetScale();
			SetScale(scale.x);
		}
		if (!Magic_IsInterpolationMode(emitter))
		{
			// rus: without interpolation a fixing step is only possible
			fElaspedTime = (float)Magic_GetUpdateTime(emitter);
		}

		// 只更新一次的发射器才自动停止
		if (!Magic_Update(emitter, fElaspedTime) && mUpdateMode == EUM_ONCE)
		{
			// eng: working of emitter is over
			SetState(ES_STOP);
		}
	}

	MAGIC_EVENT evt;
	while (Magic_GetNextEvent(&evt) == MAGIC_SUCCESS)
	{
		function_event[evt.event](&evt);
	}
}

// eng: Emitter visualization
int MP_Emitter::Render()
{
	int count = 0;
	if (mState != ES_STOP)
	{
		HM_EMITTER emitter = GetEmitter();
		if (Magic_InInterval(emitter))
		{
			MAGIC_RENDERING rendering;
			Magic_CreateFirstRenderedParticlesList(emitter, &rendering);
			int particles_count = rendering.count;
			if (particles_count)
			{
				MP_Atlas* atlas = owner->GetAtlas(rendering.texture_id);

				atlas->BeginDrawEmitter(this);

				while (particles_count)
				{
					atlas->BeginDrawParticles(particles_count);
					atlas->SetIntense(rendering.intense);

					// eng: particles drawing
					// rus: рисование частиц
					for (int i = 0; i < particles_count; ++i)
					{
						MAGIC_PARTICLE_VERTEXES vertexes;
						Magic_GetNextParticleVertexes(&vertexes);
						atlas->Draw(&vertexes);
					}

					atlas->EndDrawParticles();

					count += particles_count;
					Magic_CreateNextRenderedParticlesList(&rendering);
					particles_count = rendering.count;
				}
				atlas->EndDrawEmitter(this);
			}
		}
	}
	return count;
}

// ------------------------------------------------------------------------------------------

// eng: Class storing the files with particles copies
// rus: Класс, который хранит файлы с копиями частиц
int MP_Copy::file_count = 0;
std::string MP_Copy::file_name = "mp";
MP_Manager* MP_Copy::MP = NULL;

MP_Copy::MP_Copy(MP_Emitter* emitter)
{
	emitter_id = Magic_GetEmitterID(emitter->GetEmitter());
	reference = 0;
	file = EMPTY_STRING;
	ram = 0;
	IncReference(emitter);
}

MP_Copy::~MP_Copy()
{
	Clear();
}

// eng: Cleaning up
// rus: Очистка
void MP_Copy::Clear()
{
	if (!file.empty())
	{
		MP->RemoveFile(file.c_str());
		file = EMPTY_STRING;
	}

	if (ram)
	{
		Magic_StreamClose(ram);
		ram = 0;
	}

	reference = 0;
}

// eng: Increasing of reference count
// rus: Увеличение числа ссылок на файл
void MP_Copy::IncReference(MP_Emitter* emitter)
{
	if (!reference)
	{
		const char* temp_dir = MP->GetPathToTemp();
		if (temp_dir)
		{
			// eng: temporary folder exists
			// rus: временная папка существует
			bool repeat_select;
			do
			{
				repeat_select = false;
				// сохранение в файл
				file = temp_dir;
				file += file_name;
				static char s[20];
				sprintf(s, "%d", file_count);
				file += s;
				if (MP->RemoveFile(file.c_str()) == -1)
				{
					if (errno != ENOENT)
					{
						// eng: temporary existing was not deleted, it is necessary to choose another one
						// rus: существующий временный файл не был удален, надо выбрать другой файл
						repeat_select = true;

						// eng: select random digit from 0 to 9
						// rus: выбираем случайное число от 0 до 9
						int r = rand();
						r = r % 10;
						// eng: convert it to symbol
						// rus: превращаем его в символ
						char v = (char)r;
						v += '0';
						file_name += v;
					}
				}
			} while (repeat_select);
			++file_count;
		}
		else
		{
			// сохранение в ОЗУ
			ram = Magic_StreamOpenMemory(NULL, 0, MAGIC_STREAM_WRITE);
		}

		LoadParticles(emitter);

		++reference;
	}
	else
	{
		if (!MP->GetCopyMode())
			++reference;
	}
}

// eng: Decreasing of reference count
// rus: Уменьшение числа ссылок на файл
void MP_Copy::DecReference()
{
	if (!MP->GetCopyMode())
	{
		--reference;
		if (!reference)
			MP->DeleteCopy(this);
	}
}

// eng: Loading of particles from file to emitter
// rus: Загрузка частиц из файла в эмиттер
void MP_Copy::LoadParticles(MP_Emitter* emitter)
{
	if (ram)
	{
		Magic_StreamSetPosition(ram, 0);
		Magic_EmitterToInterval1_Stream(emitter->GetEmitter(), 1.f, ram);
	}
	else
	{
		const char* temp_file = NULL;
		if (!file.empty())
			temp_file = file.c_str();

		Magic_EmitterToInterval1(emitter->GetEmitter(), 1.f, temp_file);
	}
}

// ------------------------------------------------------------------------------------------

MP_PTC::MP_PTC()
{
	hmFile = 0;
	hmStream = 0;
	data = NULL;
}

MP_PTC::MP_PTC(const MP_PTC& other)
{
	hmFile = other.hmFile;
	hmStream = other.hmStream;
	data = other.data;
}

MP_PTC& MP_PTC::operator = (MP_PTC& other)
{
	hmFile = other.hmFile;
	hmStream = other.hmStream;
	data = other.data;
	return *this;
}

MP_PTC::~MP_PTC()
{
	Close();
}

int MP_PTC::Close()
{
	int result = MAGIC_ERROR;

	if (hmFile)
	{
		result = Magic_CloseFile(hmFile);
		hmFile = 0;
	}

	if (hmStream)
	{
		int result2 = Magic_StreamClose(hmStream);
		if (result == MAGIC_SUCCESS)
		{
			result = result2;
		}
		hmStream = 0;
	}
	TRACE_DELETE_ARRAY(data);
	return result;
}

// ------------------------------------------------------------------------------------------

// eng: Class storing the textural atlas. This class will be abstract
// rus: Класс, который хранит текстурный атлас. Этот класс будет абстрактным

MP_Manager* MP_Atlas::MP = NULL;
txMP_Device* MP_Atlas::device = NULL;

MP_Atlas::MP_Atlas(const int& width, const int& height, const char* file)
{
	atlas_width = width;
	atlas_height = height;
	file_name = EMPTY_STRING;
	if (file != NULL)
	{
		std::string strFilename = file;
		if (MP_Manager::mTextureType == TT_SWI)
		{
			int dotpos = strFilename.find_last_of('.');
			if (strFilename.substr(dotpos, strFilename.length() - dotpos) != std::string(".swi"))
			{
				strFilename = strFilename.substr(0, dotpos) + ".swi";
			}
		}
		else if (MP_Manager::mTextureType == TT_PNG)
		{
			;
		}
		file_name = strFilename;
	}
}

// ------------------------------------------------------------------------------------------

// eng: Class controlling drawing. This class will be abstract
// rus: Класс, который управляет рисованием. Этот класс будет абстрактным

MP_Manager* MP_Device::MP = NULL;

MP_Device::MP_Device()
{
	MP_Manager::device = (txMP_Device*)this;
	MP_Emitter::device = (txMP_Device*)this;
	MP_Atlas::device = (txMP_Device*)this;
}

MP_Device::~MP_Device()
{
	MP_Manager::device = NULL;
	MP_Emitter::device = NULL;
	MP_Atlas::device = NULL;
}

// eng: Destroying
// rus: Уничтожение
void MP_Device::Destroy()
{
	if (MP != NULL)
	{
		int k_atlas = MP->GetAtlasCount();
		for (int i = 0; i < k_atlas; ++i)
		{
			MP->GetAtlas(i)->Destroy();
		}
	}
}

// eng: Beginning of scene drawing
// rus: Начало отрисовки сцены
void MP_Device::BeginScene(txCamera* txcamera)
{
	if (IsLost())
	{
		Create();
#ifdef MAGIC_3D
		SetScene3d();
#endif
		MP->RestoreAtlas();
	}
}

// eng: Creating of atlas object
// rus: Создание объекта атласа
MP_Atlas* MP_Device::NewAtlas(const int& width, const int& height, const char* file)
{
	MP_Atlas* atlas = TRACE_NEW(txMP_Atlas, atlas, width, height, file);
	return atlas;
}

// ------------------------------------------------------------------------------------------

// eng: Getting identity matrix 
// rus: Возвращает единичную матрицу
void MatrixIdentity(MAGIC_MATRIX* m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i == j)
			{
				m->m[i][j] = 1;
			}
			else
			{
				m->m[i][j] = 0;
			}
		}
	}
}

// eng: Calculation of perspective projection matrix (analogue of D3DXMatrixPerspectiveFovLH from Direct3D) 
// rus: Расчет матрицы перспективной проекции (аналог D3DXMatrixPerspectiveFovLH из Direct3D)
void MatrixPerspectiveFovLH(MAGIC_MATRIX* m, const float& fovy, const float& aspect, const float& zn, const float& zf)
{
	float yScale = (float)(1 / (tan(fovy / 2.f)));
	float xScale = yScale / aspect;

	m->m[0][0] = xScale;
	m->m[1][0] = 0.f;
	m->m[2][0] = 0.f;
	m->m[3][0] = 0.f;

	m->m[0][1] = 0.f;
	m->m[1][1] = yScale;
	m->m[2][1] = 0.f;
	m->m[3][1] = 0.f;

	m->m[0][2] = 0.f;
	m->m[1][2] = 0.f;
	m->m[2][2] = zf / (zf - zn);
	m->m[3][2] = -zn * zf / (zf - zn);

	m->m[0][3] = 0.f;
	m->m[1][3] = 0.f;
	m->m[2][3] = 1.f;
	m->m[3][3] = 0.f;
}

// eng: Calculation of orthogonal projection (analogue of D3DXMatrixOrthoLH from Direct3D) 
// rus: Расчет матрицы ортогональной проекции (аналог D3DXMatrixOrthoLH из Direct3D)
void MatrixOrthoLH(MAGIC_MATRIX* m, const float& width, const float& height, const float& zn, const float& zf)
{
	m->m[0][0] = 2.f / width;
	m->m[1][0] = 0.f;
	m->m[2][0] = 0.f;
	m->m[3][0] = 0.f;

	m->m[0][1] = 0.f;
	m->m[1][1] = 2.f / height;
	m->m[2][1] = 0.f;
	m->m[3][1] = 0.f;

	m->m[0][2] = 0.f;
	m->m[1][2] = 0.f;
	m->m[2][2] = 1.f / (zf - zn);
	m->m[3][2] = zn / (zn - zf);

	m->m[0][3] = 0.f;
	m->m[1][3] = 0.f;
	m->m[2][3] = 0.f;
	m->m[3][3] = 1.f;
}

// eng: Calculation of view matrix (analogue of D3DXMatrixLookAtLH from Direct3D) 
// rus: Расчет матрицы вида (аналог D3DXMatrixLookAtLH из Direct3D)
void MatrixLookAtLH(MAGIC_MATRIX* m, MP_POSITION* peye, MP_POSITION* pat, MP_POSITION* pup)
{
	MP_POSITION right, rightn, up, upn, vec, vec2;

	vec2 = (*pat) - (*peye);

	vec = vec2;
	vec.Normalize();

	right.Cross(pup, &vec);
	up.Cross(&vec, &right);

	rightn = right;
	rightn.Normalize();

	upn = up;
	upn.Normalize();

	m->m[0][0] = rightn.x;
	m->m[1][0] = rightn.y;
	m->m[2][0] = rightn.z;
	m->m[3][0] = -peye->Dot(&rightn);
	m->m[0][1] = upn.x;
	m->m[1][1] = upn.y;
	m->m[2][1] = upn.z;
	m->m[3][1] = -peye->Dot(&upn);
	m->m[0][2] = vec.x;
	m->m[1][2] = vec.y;
	m->m[2][2] = vec.z;
	m->m[3][2] = -peye->Dot(&vec);
	m->m[0][3] = 0.0f;
	m->m[1][3] = 0.0f;
	m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}

// eng: Calculation of perspective projection matrix (analogue of D3DXMatrixPerspectiveFovRH from Direct3D)
// rus: Расчет матрицы перспективной проекции (аналог D3DXMatrixPerspectiveFovRH из Direct3D)
void MatrixPerspectiveFovRH(MAGIC_MATRIX* m, const float& fovy, const float& aspect, const float& zn, const float& zf)
{
	float yScale = (float)(1 / tan(fovy / 2.f));
	float xScale = yScale / aspect;

	m->m[0][0] = xScale;
	m->m[1][0] = 0.f;
	m->m[2][0] = 0.f;
	m->m[3][0] = 0.f;

	m->m[0][1] = 0.f;
	m->m[1][1] = yScale;
	m->m[2][1] = 0.f;
	m->m[3][1] = 0.f;

	m->m[0][2] = 0.f;
	m->m[1][2] = 0.f;
	m->m[2][2] = zf / (zn - zf);
	m->m[3][2] = zn*zf / (zn - zf);

	m->m[0][3] = 0.f;
	m->m[1][3] = 0.f;
	m->m[2][3] = -1.f;
	m->m[3][3] = 0.f;
}

// eng: Calculation of orthogonal projection (analogue of D3DXMatrixOrthoRH from Direct3D)
// rus: Расчет матрицы ортогональной проекции (аналог D3DXMatrixOrthoRH из Direct3D)
void MatrixOrthoRH(MAGIC_MATRIX* m, const float& width, const float& height, const float& zn, const float& zf)
{
	m->m[0][0] = 2.f / width;
	m->m[1][0] = 0.f;
	m->m[2][0] = 0.f;
	m->m[3][0] = 0.f;

	m->m[0][1] = 0.f;
	m->m[1][1] = 2.f / height;
	m->m[2][1] = 0.f;
	m->m[3][1] = 0.f;

	m->m[0][2] = 0.f;
	m->m[1][2] = 0.f;
	m->m[2][2] = 1.f / (zn - zf);
	m->m[3][2] = zn / (zn - zf);

	m->m[0][3] = 0.f;
	m->m[1][3] = 0.f;
	m->m[2][3] = 0.f;
	m->m[3][3] = 1.f;
}

// eng: Calculation of view matrix (analogue of D3DXMatrixLookAtRH from Direct3D)
// rus: Расчет матрицы вида (аналог D3DXMatrixLookAtRH из Direct3D)
void MatrixLookAtRH(MAGIC_MATRIX* m, MP_POSITION* peye, MP_POSITION* pat, MP_POSITION* pup)
{
	MP_POSITION xaxis, yaxis, zaxis;

	zaxis = (*peye) - (*pat);
	zaxis.Normalize();

	xaxis.Cross(pup, &zaxis);
	yaxis.Cross(&zaxis, &xaxis);

	xaxis.Normalize();
	yaxis.Normalize();

	m->m[0][0] = xaxis.x;
	m->m[1][0] = xaxis.y;
	m->m[2][0] = xaxis.z;
	m->m[3][0] = peye->Dot(&xaxis);

	m->m[0][1] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[2][1] = yaxis.z;
	m->m[3][1] = peye->Dot(&yaxis);

	m->m[0][2] = zaxis.x;
	m->m[1][2] = zaxis.y;
	m->m[2][2] = zaxis.z;
	m->m[3][2] = peye->Dot(&zaxis);

	m->m[0][3] = 0.0f;
	m->m[1][3] = 0.0f;
	m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}

// ---------------------------------------------------------------------------------------

struct TargaFileHeader
{
	char idLength;
	char colourMapType;
	char dataTypeCode;
	short colourMapOrigin;
	short colourMapLength;
	char colourMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;
};

// eng: Saves TGA for testing
// rus: Сохраняет TGA для тестирования
void SaveTGA(const short& w, const short& h, unsigned char* pixel, const char* file)
{
	char components = 4;
	TargaFileHeader fileHeader;
	memset(&fileHeader, 0, sizeof(TargaFileHeader));
	fileHeader.dataTypeCode = 2;
	fileHeader.bitsPerPixel = components * 8;
	fileHeader.width = w;
	fileHeader.height = h;
	fileHeader.imageDescriptor = components == 4 ? 8 : 0;

	FILE* f = fopen(file, "wb");
	fwrite(&fileHeader.idLength, sizeof(fileHeader.idLength), 1, f);
	fwrite(&fileHeader.colourMapType, sizeof(fileHeader.colourMapType), 1, f);
	fwrite(&fileHeader.dataTypeCode, sizeof(fileHeader.dataTypeCode), 1, f);
	fwrite(&fileHeader.colourMapOrigin, sizeof(fileHeader.colourMapOrigin), 1, f);
	fwrite(&fileHeader.colourMapLength, sizeof(fileHeader.colourMapLength), 1, f);
	fwrite(&fileHeader.colourMapDepth, sizeof(fileHeader.colourMapDepth), 1, f);
	fwrite(&fileHeader.xOrigin, sizeof(fileHeader.xOrigin), 1, f);
	fwrite(&fileHeader.yOrigin, sizeof(fileHeader.yOrigin), 1, f);
	fwrite(&fileHeader.width, sizeof(fileHeader.width), 1, f);
	fwrite(&fileHeader.height, sizeof(fileHeader.height), 1, f);
	fwrite(&fileHeader.bitsPerPixel, sizeof(fileHeader.bitsPerPixel), 1, f);
	fwrite(&fileHeader.imageDescriptor, sizeof(fileHeader.imageDescriptor), 1, f);

	fwrite(pixel, 1, w * h * components, f);

	fclose(f);
}
