void RunGame();

__declspec(dllimport) void LoaderSetup();

int main() {
	LoaderSetup();
	RunGame();
}
