#pragma once
#include "myStd.h"

//템플릿 타입에서 null타입을 대체
struct null_t {};

//struct true_type { enum _value_ { value = true }; };
//struct false_type { enum _value_ { value = false }; };

template <bool Test, class _true_type, class _false_type>
struct _If
{

};

template <class _true_type, class _false_type>
struct _If<true, _true_type, _false_type>
{
	typedef typename _true_type type;
};

template <class _true_type, class _false_type>
struct _If<false, _true_type, _false_type>
{
	typedef typename _false_type type;
};

//템플릿 메타함수 유니크 컴포넌트인지 판별
template <class _component_t>
struct is_unique_component
{
	//컴포넌트의 고유 아이디(주소값)
	typedef typename _component_t::component_identifier_t	identifier_t;
	//템플릿 타입으로 들어온 컴포넌트의 부모 컴포넌트
	typedef typename identifier_t::parent_component_t		parent_t;

	enum
	{
		//이부분 질문
		//추측 <1 조건, 2 true일경우, 3 false일경우>
		//유니크 컴포넌트인지 부모를 타고 가며 재귀적으로 확인
		value = _If<identifier_t::is_unique, std::true_type, is_unique_component<parent_t>>::type::value
	};
};
//유니크 컴포넌트 판별에 null_t 타입이 들어오면 value에 false를 넣는다
template <>
struct is_unique_component<null_t>
{
	enum { value = false };
};
//유니크 컴포넌트를 가져오는 메타함수
template <class _component_t>
struct get_unique_component
{
	typedef typename _component_t::component_identifier_t	identifier_t;
	typedef typename identifier_t::parent_component_t		parent_t;
	//현재 컴포넌트가 유니크 타입인지 검사
	//맞으면 현재 컴포넌트의 타입을 반환
	//아니면 부모를 타며 재귀호출
	typedef typename _If<identifier_t::is_unique, _component_t, typename get_unique_component<parent_t>::type>::type type;
};
template <>
struct get_unique_component<null_t>
{
	typedef null_t type;
};

//컴포넌트를 불로오는 메타함수
template <class _component_t>
struct get_component
{
	//유니크 컴포넌트면 가장 상위 유니크 컴포넌트를 반환하고
	//아니면 현재 컴포넌트를 반환
	typedef typename _If<
		is_unique_component<_component_t>::value,
		typename get_unique_component<_component_t>::type,
		_component_t>::type type;
};
//컴포넌트 식별자 생성 메타함수
template <class _component_t, class _parent_component_t, bool _is_unique>
struct _component_identifier
{
	typedef _component_t current_component_t;
	typedef _parent_component_t parent_component_t;

	enum { is_unique = _is_unique };
};
//컴포넌트 생성 매크로
//생성할 컴포넌트,부모컴포넌트,유니크여부 를 넘겨줘서 컴포넌트 클래스를 생성
//식별자의 주소를 고유 클래스 아이디로 사용
//아래 두줄 질문
#define DEFINE_COMPONENT(component_name, parent_component_name, unique_component) \
	public: \
		typedef _component_identifier<component_name, parent_component_name, unique_component> component_identifier_t; \
		static size_t getComponentId(void) { return reinterpret_cast<size_t>(&identifier); } \
		virtual const char* getComponentName_(void) { return #component_name; } \
		static const char* getComponentName(void) { return #component_name; } \
	private: \
		static component_identifier_t identifier; \
//질문
//추정 외부에서 해당 클래스의 식별자를 전역으로 선언?
#define DECLARE_COMPONENT(component_name) \
	component_name::component_identifier_t component_name::identifier;

class myComponent
{
public:
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
public:
	myComponent();
	virtual ~myComponent();
};
//여기 있으면 오류
//DECLARE_COMPONENT(myComponent);
