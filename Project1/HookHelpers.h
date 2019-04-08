#pragma once

#include <vector>
#include <type_traits>
#include <functional>

typedef struct MODULE* HMODULE;
HMODULE _gameModule;

void DetourTransactionBegin() {}
void* DetourFindFunction(HMODULE, const char* name) { return nullptr; }
void DetourAttach(void**, void*) {}
void DetourTransactionCommit() {}

enum class EventPropagation {
	Propagate,
	Consume,
};

template <auto PMF>
struct HookName;

// this is a convenience for defining the api
// the appropriate HookName specialization must supply the function name for Detours
#define DEFINE_METHOD(PMF) \
template <> \
struct HookName<&PMF> { \
	static constexpr const char Name[] = #PMF; \
}

template <typename T, T>
struct HookInvoker;

// partial specialization for member functions; could also be specialized for free functions
template <typename R, typename C, typename... A, R(C::*PMF)(A...)>
class HookInvoker<R(C::*)(A...), PMF> {
public:
	// mod handler function
	typedef R HandlerSignature(EventPropagation&, C*, A...);

	// support arbitrary context for handlers
	typedef std::function<HandlerSignature> Handler;

	// if std::function seems like too much overhead, this typedef can be used for raw function pointers instead
	// typedef HandlerSignature* Handler;

private:
	// detoured function
	typedef R __fastcall HookType(C*, A...);

	static std::vector<Handler> handlers;
	static void* original;

	static HookType* GetApply(std::true_type) {
		return &ApplyVoid;
	}

	static HookType* GetApply(std::false_type) {
		return &Apply;
	}

	static R __fastcall Apply(C* thiz, A... args) {
		auto propagate = EventPropagation::Propagate;

		R ret;

		for(auto&& handler : handlers)
			ret = handler(propagate, thiz, args...);

		if(propagate != EventPropagation::Consume)
			return ((HookType*)original)(thiz, args...);

		return ret;
	}

	static void __fastcall ApplyVoid(C* thiz, A... args) {
		auto propagate = EventPropagation::Propagate;

		for(auto&& handler : handlers)
			handler(propagate, thiz, args...);

		if(propagate != EventPropagation::Consume)
			((HookType*)original)(thiz, args...);
	}

	static void InstallHook() {
		// only if not already installed
		if(!original) {
			DetourTransactionBegin();
			original = DetourFindFunction(_gameModule, HookName<PMF>::Name);
			DetourAttach(&original, (void*)GetApply(std::is_same<R, void>{}));
			DetourTransactionCommit();
		}
	}

public:
	static void Subscribe(Handler handler) {
		InstallHook();
		handlers.push_back(handler);
	}
};

template <typename R, typename C, typename... A, R(C::*PMF)(A...)>
std::vector<typename HookInvoker<R(C::*)(A...), PMF>::Handler> HookInvoker<R(C::*)(A...), PMF>::handlers;

template <typename R, typename C, typename... A, R(C::*PMF)(A...)>
void* HookInvoker<R(C::*)(A...), PMF>::original;

template <auto HookableFunction>
void Subscribe(typename HookInvoker<decltype(HookableFunction), HookableFunction>::Handler handler) {
	HookInvoker<decltype(HookableFunction), HookableFunction>::Subscribe(handler);
}
