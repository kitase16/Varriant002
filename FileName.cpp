#include <iostream>
#include <memory>

template<class T>
struct Type {
	typedef T type;
};

template<class T>
struct Variant {

	Variant(const Variant<T>& In) :Ty{ In.Ty }, val{ In.val } {}
	Variant(const T& In) :Ty{ Type<T>{} }, val{ In } {}
	Type<T> Ty;
	decltype(Ty)::type val;
	typedef Variant<T> Self;
};

template<class T>
struct VarT{
	VarT(const T& In) :SV{ std::make_shared<Variant<T>>(In) } {}
	std::shared_ptr<Variant<T>> SV;
	
};

template<class U,class... T>
VarT<U> mkVarT(const T&... In) {
	return VarT<U>{In...};
}

template<class... T>
class TypeList:T... {
public:
	template<class U>
	Cast() {
		return static_cast<U*>(this);
	}

};

template<class U,class... T>
std::shared_ptr<U> mkHolder(const T&... In) {

	TypeList<U,T...> TL;

	return std::make_shared<U,decltype(TL)>(In...);
}

struct Var {
	VarT<void> Val;
};

template<class U, class... T>
Var mkVar(const T&... in) {
	return Var{ mkVarT<U>(in...) };
}

int main() {
	VarT<int> v = mkVarT<int>(5);

	std::cout << v.SV->val << std::endl;

	//Var v2 = mkVar<int>(10);
	//std::cout << v2.v.SV->val << std::endl;

	Var v3 = mkVar<int>(10);
	std::cout << v3.Val.SV->val << std::endl;
	return 0;
}