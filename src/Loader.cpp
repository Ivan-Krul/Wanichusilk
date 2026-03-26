#include "Loader.h"
#include "Logger.h"

void Loader::PushLoadInQueue(IResourceParamConvertor<ResourceLoadParams>* load, IResourceManager* manager) {
    mLoadErrNum = -2;

    ResourceIndexer ri;
    ri.mgr_ptr = manager;
    ri.payload.load = load->to_param();
	ri.action = ri.Load;

    maResMgr.push_back(ri);
}

void Loader::PushConvertInQueue(IResourceParamConvertor<ResourceConvertParams>* convert, IResourceManager* manager, IResourceManager* from_manager, LockerIndex from_index) {
	mLoadErrNum = -2;
	
	ResourceIndexer ri;
    ri.mgr_ptr = manager;
    ri.payload.convert.params = convert->to_param();
	ri.payload.convert.from_manager = from_manager;
	ri.payload.convert.from_index = from_index;
	ri.action = ri.Convert;

    maResMgr.push_back(ri);
}

const char* Loader::GetResourcePath(size_t index) const {
	if(maResMgr.size() > index) return "";
	if(maResMgr.at(index).action != ResourceIndexer::Load) return "";
	
	return maResMgr.at(index).payload.load.path;
}

IResourceManager* Loader::GetRequiredInterface(ResourceManagerAttribute attr) const noexcept {
    for (auto& idx : maResMgr) {
        if (static_cast<short>(idx.mgr_ptr->GetAttribute() & attr)) return idx.mgr_ptr;
    }
    return nullptr;
}

void Loader::Load() {
    mLoadErrNum = -2;
    mProgress = 0;
    mLoadThread = std::thread(&Loader::loadMain, this);
    mLoadThread.detach();
}

void Loader::Convert() {
    mLoadErrNum = -2;
    mProgress = 0;
    mLoadThread = std::thread(&Loader::convertMain, this);
    mLoadThread.detach();
}

bool Loader::Preprocess() {
    for (auto i = 0; i < maResMgr.size(); i++) {
        auto& res = maResMgr[i];
        if (short((res.mgr_ptr->GetAttribute()) & ResourceManagerAttribute::Preprocesser)) {
            if (dynamic_cast<IResourcePreprocesser*>(res.mgr_ptr)->RequestResourcePreprocess(res.index)) {
                mLoadErrNum = i;
                return true;
            }
        }
    }
    return false;
}

void Loader::Clean() {
    for (auto i = 0; i < maResMgr.size(); i++) {
        auto& res = maResMgr[i];
        res.mgr_ptr->RequestResourceClean(res.index);
    }
    maResMgr.clear();

    mLoadErrNum = -2;
}

void Loader::loadMain() {
    LockerIndex li_res;
    for (mProgress = 0; mProgress < maResMgr.size(); mProgress++) {
        auto& res = maResMgr[mProgress];
		if(res.action != res.Load) continue;
        li_res = res.mgr_ptr->RequestResourceLoad(res.payload.load);

        if (li_res == -1) {
            mLoadErrNum = mProgress;
            mProgress = -1;
            return;
        }

        res.index = li_res;
    }

    mProgress = -1;
    mLoadErrNum = -1;
}

void Loader::convertMain() {
    LockerIndex li_res;
    for (mProgress = 0; mProgress < maResMgr.size(); mProgress++) {
        auto& res = maResMgr[mProgress];
		if(res.action != res.Convert) continue;
		IResourceConvertor* res_mgr_conv_ptr = nullptr;
		if(!(res_mgr_conv_ptr = dynamic_cast<IResourceConvertor*>(res.mgr_ptr))) {
			Logger log(DEFAULT_LOG_PATH);
			log.logErrorIn(__FUNCTION__, "Incompatable convertion manager was fed.");
			continue;
		}
        li_res = res_mgr_conv_ptr->RequestResourceConvert(res.payload.convert.from_manager, res.payload.convert.from_index, res.payload.convert.params);

        if (li_res == -1) {
            mLoadErrNum = mProgress;
            mProgress = -1;
            return;
        }

        res.index = li_res;
    }

    mProgress = -1;
    mLoadErrNum = -1;
}
