#include "GameApi.h"

void OnBeginPlay(EventPropagation&, AFGPlayerController* thiz) {
}

void OnBeginPlayWrongArguments(EventPropagation&, AFGPlayerController* thiz, int what) {
}

bool OnInputKey(EventPropagation&, UPlayerInput* thiz, FKey key, EInputEvent event, float amount, bool bGamepad) {
	return true;
}

void ModSetup() {
	// regular functions
	Subscribe<&AFGPlayerController::BeginPlay>(OnBeginPlay);

	// return values
	Subscribe<&UPlayerInput::InputKey>(OnInputKey);

	// type-checking: the following will not compile because the class type is wrong
	// Subscribe<&AFGCharacterPlayer::BeginPlay>(OnBeginPlay);

	// type-checking: the following will not compile because the arguments are wrong
	// Subscribe<&AFGPlayerController::BeginPlay>(OnBeginPlayWrongArguments);

	int x = 15;

	// lambdas with captures
	Subscribe<&AFGCharacterPlayer::BeginPlay>([=](EventPropagation&, AFGCharacterPlayer* thiz) {
		int y = x + 5;
	});

	// auto lambdas
	Subscribe<&AFGPlayerController::EnterChatMessage>([](auto& propagation, auto thiz, auto msg) {
		static_assert(std::is_same_v<decltype(thiz), AFGPlayerController*>, "type mismatch!");
		static_assert(std::is_same_v<decltype(msg), FString*>, "type mismatch!");
	});
}

int main() {
	ModSetup();
}