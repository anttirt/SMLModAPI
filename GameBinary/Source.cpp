#include <iostream>

struct __declspec(dllexport) FString {};

class __declspec(dllexport) AActor {};
class __declspec(dllexport) UDamageType {};
class __declspec(dllexport) AController {};

class __declspec(dllexport) UObject {

};

class __declspec(dllexport) AFGCharacterBase : public UObject {
public:
	void __fastcall BeginPlay(){}
	void __fastcall GetHealthComponent(struct FFrame*, void* const){}
};

class __declspec(dllexport) AFGCharacterPlayer : public AFGCharacterBase
{
public:
	void __fastcall BeginPlay(){}
};

class __declspec(dllexport) AFGPlayerController {
public:
	void __fastcall BeginPlay(){}
	void __fastcall EnterChatMessage(FString *inMessage){}
};

class __declspec(dllexport) UFGHealthComponent {
public:
	void __fastcall TakeDamage(AActor* damagedActor, float damageAmount, UDamageType *damageType, AController *instigatedBy, AActor *damageCauser){}
};

enum FKey : int {};

enum EInputEvent : int {};

class __declspec(dllexport) UPlayerInput {
public:
	bool __fastcall InputKey(FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad){
		std::cout << "GameBinary.dll!InputKey()" << std::endl;
		return true;
	}
};

__declspec(dllexport) void RunGame() {
	AFGCharacterPlayer player;
	player.BeginPlay();

	UPlayerInput input;
	bool result = input.InputKey(FKey{}, EInputEvent{}, 0.5f, false);
	std::cout << "InputKey returned " << (result ? "true" : "false") << std::endl;
}