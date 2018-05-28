#ifndef _ENGINE_CALLBACK_H_
#define _ENGINE_CALLBACK_H_

class TextureAnimWindow;
class VideoWindow;
// isBreak表示是否是中断了序列帧的播放而导致的调用回调,下同
typedef void(*TextureAnimPlayEndCallback)(TextureAnimWindow* window, void* userData, const bool& isBreak);
// isBreak表示是否是中断了视频的播放而导致的调用回调,下同
typedef void(*VideoPlayEndCallback)(VideoWindow* window, void* userData, const bool& isBreak);

// 录音机的回调,在接收到录音数据时调用
class txWavRecorder;
typedef void(*RecordCallback)(txWavRecorder* recorder, void* userData, char* data, const int& dataSize);

// 动作关键帧的回调函数,curAnimTime为播放到的帧数,不是播放的秒数,cutAnim为true表示是因为动作被切换了而调用的该回调函数
typedef void(*AnimKeyFrameCallback) (void* user_data, const int& aditionalParam, const std::string& animName, const std::string& entityName, const float& curAnimTime, const float& animSpeed, const bool& cutAnim);

// 行为回调
class txAction;
typedef void(*ActionCallback)(txAction* action, void* userData);

// 组件回调
class txComponentKeyFrame;
typedef void(*KeyFrameCallback)(txComponentKeyFrame* component, void* userData, const bool& breakTremling, const bool& done);

// 窗口事件回调
class txTexture;
class Window;
typedef void(*WindowEventCallback)(Window* window, void* userData);

// 布局异步加载回调
class Layout;
typedef void(*LayoutAsyncDone)(Layout* layout, void* userData);

// 线程回调
typedef bool(*CustomThreadCallback)(void* args);

#endif