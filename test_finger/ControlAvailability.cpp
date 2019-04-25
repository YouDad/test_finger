//#pragma once
#include"stdafx.h"
//#include<vector>
//#include<map>
//#define foreach(TYPE,IT,CONTAINER) \
//	for(TYPE::iterator IT=CONTAINER.begin();IT!=CONTAINER.end();IT++)
////应用了观察者模式
//
////观察者必须具有的接口:控制控件可用性的接口
//interface IUpdateCtrl{
//public:
//	void Update(action a);
//};
//
////观察者分成不同角色
//template <typename T>
//class UpdateRole : public IUpdateCtrl
//{
//public:
//	void Update(action a){
//		if(m.count(a)){
//			bool b=m[a];
//			foreach(Ctrls,it,v){
//				(*it)->EnableWindow(b);
//			}
//			foreach(Ctrls,it,rev){
//				(*it)->EnableWindow(!b);
//			}
//		}
//	}
//	void Register(CWnd* ctrl,bool rev=false){
//		if(rev){
//			revVector.push_back(ctrl);
//		}else{
//			Vector.push_back(ctrl);
//		}
//	}
//protected:
//	std::map<action,bool> m;
//	typedef std::vector<CWnd*> Ctrls;
//	Ctrls Vector,revVector;
//public:
//	static T* GetInstance(){
//		if (s_instance.get()==NULL){
//			static HANDLE mutex=CreateMutex(NULL,FALSE,NULL);
//			WaitForSingleObject(mutex,INFINITE);
//			if (s_instance.get()==NULL){
//				s_instance.reset(new T());
//			}
//			ReleaseMutex(mutex);
//		}
//		return s_instance;
//	}
//protected:
//	UpdateRole(){}
//	~UpdateRole(){}
//	static std::auto_ptr<T> s_instance;
//private:
//	UpdateRole(const UpdateRole&);
//	UpdateRole& operator =(const UpdateRole&);
//};
//template <typename T>std::auto_ptr<T> UpdateRole<T>::s_instance;
//
//#define ObserverDefineBegin(name)		\
//class name : public UpdateRole<name>{	\
//private:								\
//	~name(){}							\
//	friend UpdateRole<name>;			\
//	friend std::auto_ptr<name>;			\
//	name(const name&);					\
//	name& operator =(const name&);		\
//	name()
//
//ObserverDefineBegin(InitialObserver){
//	//2.每一类角色负责对何种action做出反应
//	m[actInit]=true;
//	CWnd* Array[]={cmbBaud,cmbWay,0};
//	CWnd* revArray[]={editAddress,editPassword,editAddressSet,editPasswordSet,
//		editLightTime,editSensitivity,cmbBaudSet,cmbChipType,btnRawImage,btnTestImage,
//		btnContinueImage,btnContinueBackGroundImage,btnSetCmos,btnSetBaud,btnSetPassword,
//		btnSetAddress,btnReadReg,btnWriteReg,chkTimeoutContinue,0};
//	for(CWnd**i=Array;*i;i++){
//		Register(*i);
//	}
//	for(CWnd**i=revArray;*i;i++){
//		Register(*i,true);
//	}
//}};
//
//ObserverDefineBegin(ConnectObserver){
//	//2.每一类角色负责对何种action做出反应
//}};
//	
//#undef ObserverDefineBegin(name)
//
////被观察者类
//class UpdateBroadcast{
//public:
//	//对观察者广播更新
//	void Update(action a){
//		foreach(IUpdater,it,v){
//			(*it)->Update(a);
//		}
//	}
//	static UpdateBroadcast* getIns(){
//		if(!instance){
//			instance=new UpdateBroadcast();
//		}
//		return instance;
//	}
//private:
//	typedef std::vector<IUpdateCtrl*> IUpdater;
//	IUpdater v;
//	static UpdateBroadcast* instance;
//	UpdateBroadcast(){
//		//1.广播对象的注册
//		Register(InitialObserver::GetInstance());
//		Register(ConnectObserver::GetInstance());
//	}
//	UpdateBroadcast(const UpdateBroadcast&){}
//	UpdateBroadcast& operator=(const UpdateBroadcast&){}
//	void Register(IUpdateCtrl* iUpdateCtrl){
//		v.push_back(iUpdateCtrl);
//	}
//};
