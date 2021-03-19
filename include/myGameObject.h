#pragma once
#include "myStd.h"

#pragma region ComponentMetaFunc



//템플릿 타입에서 null타입을 대체
struct null_t {};

//struct true_type { enum _value_ { value = true }; };
//struct false_type { enum _value_ { value = false }; };

template <bool Test, class True_Meta_Func, class False_Meta_Func>
struct _If
{

};

template <class True_Meta_Func, class False_Meta_Func>
struct _If<true, True_Meta_Func,False_Meta_Func>
{
	typedef typename True_Meta_Func type;
};

template <class True_Meta_Func, class False_Meta_Func>
struct _If<false, True_Meta_Func, False_Meta_Func>
{
	typedef typename False_Meta_Func type;
};

//템플릿 메타함수 유니크 컴포넌트인지 판별
template <class Component_T>
struct Is_Unique_Component
{
	//컴포넌트의 고유 아이디(주소값)
	typedef typename Component_T::Component_Identifier_T			Identifier_T;
	//템플릿 타입으로 들어온 컴포넌트의 부모 컴포넌트
	typedef typename Identifier_T::Parent_Component_T				Parent_T;

	enum
	{
		//이부분 질문
		//추측 <1 조건, 2 true일경우, 3 false일경우>
		//유니크 컴포넌트인지 부모를 타고 가며 재귀적으로 확인
		value = _If<Identifier_T::is_unique, std::true_type, Is_Unique_Component<Parent_T>>::type::value
	};
};
//유니크 컴포넌트 판별에 null_t 타입이 들어오면 value에 false를 넣는다
template <>
struct Is_Unique_Component<null_t>
{
	enum { value = false };
};
//유니크 컴포넌트를 가져오는 메타함수
template <class Component_T>
struct Get_Unique_Component
{
	typedef typename Component_T::Component_Identifier_T	Identifier_T;
	typedef typename Identifier_T::Parent_Component_T		Parent_T;
	//현재 컴포넌트가 유니크 타입인지 검사
	//맞으면 현재 컴포넌트의 타입을 반환
	//아니면 부모를 타며 재귀호출
	typedef typename _If<Identifier_T::is_unique, Component_T, typename Get_Unique_Component<Parent_T>::type>::type type;
};
template <>
struct Get_Unique_Component<null_t>
{
	typedef null_t type;
};

//컴포넌트를 불로오는 메타함수
template <class Component_T>
struct Get_Component
{
	//유니크 컴포넌트면 가장 상위 유니크 컴포넌트를 반환하고
	//아니면 현재 컴포넌트를 반환
	typedef typename _If<
		Is_Unique_Component<Component_T>::value,
		typename Get_Unique_Component<Component_T>::type,
		Component_T>::type type;
};
//컴포넌트 식별자 생성 메타함수
template <class Component_T, class Parent_Component_T, bool _is_unique>
struct Component_Identifier
{
	typedef Component_T Current_Component_T;
	typedef Parent_Component_T Parent_Component_T;

	enum { is_unique = _is_unique };
};
//컴포넌트 생성 매크로
//생성할 컴포넌트,부모컴포넌트,유니크여부 를 넘겨줘서 컴포넌트 클래스를 생성
//식별자의 주소를 고유 클래스 아이디로 사용
//아래 두줄 질문

#define DEFINE_COMPONENT(component_name, parent_component_name, unique_component) \
	public: \
		typedef Component_Identifier<component_name, parent_component_name, unique_component> Component_Identifier_T; \
		static size_t GetComponentId(void) { return reinterpret_cast<size_t>(&identifier); } \
		virtual const char* GetComponentName_(void) { return #component_name; } \
		static const char* GetComponentName(void) { return #component_name; } \
	private: \
		static Component_Identifier_T identifier; \
//질문
//추정 외부에서 해당 클래스의 식별자를 전역으로 선언?
#define DECLARE_COMPONENT(component_name) \
	component_name::Component_Identifier_T component_name::identifier;
	
#pragma endregion

class myGameObject;
class myTransform;

class myComponent
{
public:
	myGameObject*	m_pGameObject;
	myTransform*	m_pTransform;
	bool			m_isActive;
	bool			m_isRender;
	DEFINE_COMPONENT(myComponent, null_t, false);
public:
	virtual bool	Init();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	PostFrame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual void	Update();
	virtual void	Reset();
	virtual bool	Action();
	virtual bool	Release();
	virtual void	Set(myGameObject* pGameObject)
	{
		m_pGameObject = pGameObject;
	}
public:
	myComponent()
	{
		m_isActive = true;
		m_isRender = true;
	};
	virtual ~myComponent() {}
};
//여기 있으면 오류
//DECLARE_COMPONENT(myComponent);

class myTransform : public myComponent
{
public:
	DEFINE_COMPONENT(myComponent, myComponent, true);
public:
	float		m_fSpeed;
public:
	Vector3		m_vPos;
	Quaternion	m_qRot;
	Vector4		m_vRot;
	Vector3		m_vScale;
	Vector3 	m_vTarget;
	Vector3		m_vLook;
	Vector3		m_vUp;
	Vector3		m_vRight;
public:
	Vector3		m_vLocalPos;
	Quaternion	m_qLocalRot;
	Vector4		m_vLocalRot;
	Vector3		m_vLocalScale;
	Vector3		m_vLocalTarget;
	Vector3		m_vLocalLook;
	Vector3		m_vLocalUp;
	Vector3		m_vLocalRight;
public:
	Matrix		m_matRot;
	Matrix		m_matScale;
	Matrix		m_matWorld;
	Matrix		m_matLocal;
	Matrix		m_matLocalRot;
	Matrix		m_matLocalScale;
	Matrix		m_matView;
	Matrix		m_matProj;
public:
	virtual bool	SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj);
	virtual void	UpdateVector();
	virtual void	SetPos(Vector3 p);
	virtual void	SetTarget(Vector3 p);
public:
	void FrontMovement(float fDir = 1.0f);
	void RightMovement(float fDir = 1.0f);
	void UpMovement(float fDir = 1.0f);
	void FrontBase(float fDir);
	void RightBase(float fDir);
	void UpBase(float fDir);
	void LookAt(Vector3 vTarget);
public:
	virtual bool	Init() override;
	virtual bool	PreFrame() override;
	virtual bool	Frame() override;
	virtual bool	PostFrame() override;
	virtual bool	PreRender() override;
	virtual bool	Render() override;
	virtual bool	PostRender() override;
	virtual void	Update() override;
	virtual void	Reset() override;
	virtual bool	Action() override;
	virtual bool	Release() override;
	myTransform()
	{
		m_fSpeed = 30.0f;
		m_vPos = Vector3(0.0f, 0.0f, 0.0f);
		m_qRot = Quaternion::Identity;
		m_vRot = Vector4::Zero;
		m_vScale = Vector3(1.0f, 1.0f, 1.0f);
		m_vLook = Vector3(0.0f, 0.0f, 1.0f);
		m_vTarget = m_vLook;
		m_vUp = Vector3(0.0f, 1.0f, 0.0f);
		m_vRight = Vector3(1.0f, 0.0f, 0.0f);
		m_vLocalPos = Vector3(0.0f, 0.0f, 0.0f);
		m_qLocalRot = Quaternion::Identity;
		m_vLocalRot = Vector4::Zero;
		m_vLocalScale = Vector3(1.0f, 1.0f, 1.0f);
		m_vLocalLook = Vector3(0.0f, 0.0f, 1.0f);
		m_vLocalTarget = m_vLook;
		m_vLocalUp = Vector3(0.0f, 1.0f, 0.0f);
		m_vLocalRight = Vector3(1.0f, 0.0f, 0.0f);

		m_matRot = Matrix::Identity;
		m_matScale = Matrix::Identity;
		m_matWorld = Matrix::Identity;
		m_matLocal = Matrix::Identity;
		m_matLocalRot = Matrix::Identity;
		m_matLocalScale = Matrix::Identity;

		m_matView = Matrix::Identity;
		m_matProj = Matrix::Identity;
	}
	~myTransform() {}
};

class myGameObject : public myComponent
{
public:
	DEFINE_COMPONENT(myGameObject, myComponent, true);
public:
	T_STR			m_strName;
	T_STR			m_strTag;
private:
	//게임오브젝트 언오더맵으로 바꿔보기
	myGameObject*									m_pParent;
	unordered_map<wstring, myGameObject*>			m_Childs;
	unordered_map<wstring, myGameObject*>::iterator	m_ObjIter;
	unordered_map<size_t, myComponent*>				m_ComponentList;
	unordered_map<size_t, myComponent*>::iterator	m_ComIter;
public:
	static myGameObject* CreateGameObject()
	{
		return new myGameObject;
	}
	static myGameObject* CreateGameObject(const TCHAR* szName)
	{
		return new myGameObject(szName);
	}
	void SetObjectName(const TCHAR* szName)
	{
		m_strName = szName;
	}
public:
	virtual myGameObject* Clone();
	virtual bool	Init();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	PostFrame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual void	Update();
	virtual void	Reset();
	virtual bool	Action();
	virtual bool	Release();
public:
	myGameObject*			Add(wstring strName = L"");
	myGameObject*			GetGameObject(wstring strName);
	list<myGameObject*>*	GetGameObjects(wstring strName);
private:
	void					SetParent(myGameObject* pParent);
public:

	template <class Component_T>
	void InsertComponent(Component_T* component)
	{
		//--------------------------------------------------------
		// 컴포넌트 식별자 ID 취득
		//--------------------------------------------------------
		size_t componentId = Get_Component<Component_T>::type::GetComponentId();

		//--------------------------------------------------------
		// 컴포넌트를 컨테이너에 삽입
		//--------------------------------------------------------
		component->Set(this);
		component->m_pTransform = this->m_pTransform;
		this->m_ComponentList[componentId] = component;
 	}
	template <class Component_T>
	Component_T* GetComponent()
	{
		//--------------------------------------------------------
		// 컴포넌트 식별자 ID 취득
		//--------------------------------------------------------
		size_t componentId = Get_Component<Component_T>::type::GetComponentId();

		//--------------------------------------------------------
		// 컴포넌트가 컨테이너에 존재하지 않다면 nullptr 리턴
		//--------------------------------------------------------
		if (this->m_ComponentList.find(componentId) == this->m_ComponentList.end())
			return nullptr;

		//--------------------------------------------------------
		// 컴포넌트가 존재한다면 컴포넌트 리턴
		//--------------------------------------------------------
		return reinterpret_cast<Component_T*>(this->m_ComponentList[componentId]);
	}
public:
	myGameObject() 
	{ 
		m_pParent = nullptr;
		m_pTransform = new myTransform;
		InsertComponent(m_pTransform);
	}
	myGameObject(const TCHAR* szName)
	{
		m_strName = szName;
		m_pTransform = new myTransform;
		InsertComponent(m_pTransform);
	}
	~myGameObject() {}
};

