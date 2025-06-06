// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "mem.h"
#include <vector>
// wtf is HMODULE?
DWORD WINAPI HackThread(HMODULE hModule) // making a thread, to module handle
{
    // console creation
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Og for a fee stay sippin fam\n";

    // Kinda readable even if you have never developed anything
    // we are getting our module base using getmodulehandle
    // get module handle is looking at (L"gamename.exe/gameassembly.dll")
    // Why are we casting getmodulehandle to uintptr_t? 
    uintptr_t modbase = (uintptr_t)GetModuleHandle(L"ac_client.exe");


    // stuff we wanna mess with
    bool bHealth = 0, bAmmo = 0, bRecoil = 0, bCooldown = 0;
    // Loop for hacky
    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break; // this just ends the loop closing dll?
        }

        if (GetAsyncKeyState(VK_F1) & 1)
        {
            bHealth = !bHealth;

        }  
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            bAmmo = !bAmmo;
         
            /*if (bAmmo)
            {
                mem::patch((BYTE*)(modbase + 0x637E9), (BYTE*)"\xFF\x06", 2);
            }
            else
            {
                mem::patch((BYTE*)(modbase + 0x637E9), (BYTE*)"\xFF\x0E", 2);
            }
            */
        }
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            bCooldown = !bCooldown;
        }
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            bRecoil = !bRecoil;
            if (bRecoil)
            {
                // Nop memory for 10 bytes
                // Barney style - Hey compiler, i know theres running code here
                // Tell this code it now has no operation
                mem::nop((BYTE*)(modbase + 0x63786), 10);
            }
            else
            {
                // Replace that no operation back to its original code bytes
                mem::patch((BYTE*)(modbase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }

        // Permenant write, Freeze stuff
        uintptr_t* localPlayer = (uintptr_t*)(modbase + 0x10f4f4);
        
        if (localPlayer)
        {
            if (bHealth)
            {
                *(int*)(*localPlayer + 0xf8) = 9999;
                // breaking this down for myself

                // (*localplayer + 0xf8) Hey c++, Dereference our localplayer 
                // into a memory address, Then add 0xf8 to it -> This is our health address dereferenced
                // 
                // (int*) Hey c++, the value stored here is an integer
                //
                // *(int*)(*localplayer + 0xf8); Dereference the value that is being held in the address
                //
                // *(int*)(*localplayer + 0xf8) = 9999; Set the value to what i want (9999)




                // ChatGPT notes after pasting the function and asking if my notes where correct

                // *(int*)(*localPlayer + 0xf8) = 9999;

                // Step 1: *localPlayer
                //   → Dereference the pointer to get the address of the player object
                //   → Let's say this gives us: 0x500000

                // Step 2: *localPlayer + 0xf8
                //   → Move forward 0xF8 bytes into the player object
                //   → This is the health field (memory address: 0x5000F8)

                // Step 3: (int*)(...)
                //  → Tell C++: “This memory location holds an int”

                // Step 4: *(int*)(...)
                //   → Dereference it again to read/write the integer stored at 0x5000F8
                //   -> This is pretty much readprocessmemory
                // 
                // 
                // Step 5: = 9999;
                //   → Write the value 9999 directly into memory at that location
                //   → This changes the player’s health to 9999
                // Writeprocessmemory

            }
        }
        if (bAmmo)
        {
            uintptr_t ammoaddr = mem::FindDMAAddy(modbase + 0x10f4f4, { 0x374, 0x14, 0x0 }); 
            int* ammovalue = (int*)ammoaddr;
            *ammovalue = 9999;
         
        }
        if (bCooldown)
        {

            uintptr_t cooldown = mem::FindDMAAddy(modbase + 0x10f4f4, { 0x374, 0x18, 0x0 });
            int* nocooldown = (int*)cooldown;
            *nocooldown = 0;
        }

       Sleep(5);
    }
    //Cleanup, Eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )



{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // YOU ONLY want to create a thread here.
        // thread start routine just makes compiler happy :)
        // IMPORTANT! lpstartaddress = address of the thread
        // hmodule - Passes the handle to the game through hModule
        // - Pretty much whatever you need to pass to your DLL
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

