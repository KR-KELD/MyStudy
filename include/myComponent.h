#pragma once
#include "myStd.h"

//���ø� Ÿ�Կ��� nullŸ���� ��ü
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

//���ø� ��Ÿ�Լ� ����ũ ������Ʈ���� �Ǻ�
template <class _component_t>
struct is_unique_component
{
	//������Ʈ�� ���� ���̵�(�ּҰ�)
	typedef typename _component_t::component_identifier_t	identifier_t;
	//���ø� Ÿ������ ���� ������Ʈ�� �θ� ������Ʈ
	typedef typename identifier_t::parent_component_t		parent_t;

	enum
	{
		//�̺κ� ����
		//���� <1 ����, 2 true�ϰ��, 3 false�ϰ��>
		//����ũ ������Ʈ���� �θ� Ÿ�� ���� ��������� Ȯ��
		value = _If<identifier_t::is_unique, std::true_type, is_unique_component<parent_t>>::type::value
	};
};
//����ũ ������Ʈ �Ǻ��� null_t Ÿ���� ������ value�� false�� �ִ´�
template <>
struct is_unique_component<null_t>
{
	enum { value = false };
};
//����ũ ������Ʈ�� �������� ��Ÿ�Լ�
template <class _component_t>
struct get_unique_component
{
	typedef typename _component_t::component_identifier_t	identifier_t;
	typedef typename identifier_t::parent_component_t		parent_t;
	//���� ������Ʈ�� ����ũ Ÿ������ �˻�
	//������ ���� ������Ʈ�� Ÿ���� ��ȯ
	//�ƴϸ� �θ� Ÿ�� ���ȣ��
	typedef typename _If<identifier_t::is_unique, _component_t, typename get_unique_component<parent_t>::type>::type type;
};
template <>
struct get_unique_component<null_t>
{
	typedef null_t type;
};

//������Ʈ�� �ҷο��� ��Ÿ�Լ�
template <class _component_t>
struct get_component
{
	//����ũ ������Ʈ�� ���� ���� ����ũ ������Ʈ�� ��ȯ�ϰ�
	//�ƴϸ� ���� ������Ʈ�� ��ȯ
	typedef typename _If<
		is_unique_component<_component_t>::value,
		typename get_unique_component<_component_t>::type,
		_component_t>::type type;
};
//������Ʈ �ĺ��� ���� ��Ÿ�Լ�
template <class _component_t, class _parent_component_t, bool _is_unique>
struct _component_identifier
{
	typedef _component_t current_component_t;
	typedef _parent_component_t parent_component_t;

	enum { is_unique = _is_unique };
};
//������Ʈ ���� ��ũ��
//������ ������Ʈ,�θ�������Ʈ,����ũ���� �� �Ѱ��༭ ������Ʈ Ŭ������ ����
//�ĺ����� �ּҸ� ���� Ŭ���� ���̵�� ���
//�Ʒ� ���� ����
#define DEFINE_COMPONENT(component_name, parent_component_name, unique_component) \
	public: \
		typedef _component_identifier<component_name, parent_component_name, unique_component> component_identifier_t; \
		static size_t getComponentId(void) { return reinterpret_cast<size_t>(&identifier); } \
		virtual const char* getComponentName_(void) { return #component_name; } \
		static const char* getComponentName(void) { return #component_name; } \
	private: \
		static component_identifier_t identifier; \
//����
//���� �ܺο��� �ش� Ŭ������ �ĺ��ڸ� �������� ����?
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
//���� ������ ����
//DECLARE_COMPONENT(myComponent);
