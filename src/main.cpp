#include "Plugin.h"
#include "Data_Source.h"
#include <stdexcept>

void (*Create_Level)(std::string Name);
void (*Save_Level)(std::string Name, std::string Snapshot);
std::string (*Load_Level)(std::string Name);

Plugin Module;
bool Module_Debug = false;

void (*Debug)(bool Enable);
void (*Set_Paths)(std::string Resources);
void (*Open)(std::string Name);
void (*Close)(std::string Name);
void (*Use)(std::string Name);
void (*Read)(std::string File);
void (*Write)(std::string Name, std::string File);
void (*Exists)(std::string File);
void (*Delete)(std::string File);
void (*Set)(std::string Key, std::string Value);
void (*Get)(std::string Key);
void (*Insert)(std::string Collection, std::string Key, std::string Value);
void (*Update)(std::string Collection, std::string Key, std::string Value);
void (*Find)(std::string Collection, std::string Key);
void (*List)(std::string Collection);
void (*Remove)(std::string Collection, std::string Key);
void (*Begin)();
void (*Commit)();
void (*Rollback)();
void (*Backup)(std::string File);
void (*Restore)(std::string File);
void (*Export)(std::string File);
void (*Import)(std::string File);
void (*Save)(std::string Name);
void (*Load)(std::string Name);
void (*Execute_SQL)(std::string SQL);
std::string (*Try_SQL)(std::string SQL);
void (*Open_Memory)(std::string Name);
std::string (*Get_Field)(std::string Table, std::string Key_Column, std::string Key, std::string Field);
void (*Set_Field)(std::string Table, std::string Key_Column, std::string Key, std::string Field, std::string Value);

template<typename T>
void Assign(std::string Name, T &Function)
{
    Module.Assign(Name, Function);
}

extern "C" void Init(std::string Name)
{
    Module.Load(Name);
    Assign("Create_Level", Create_Level);
    Assign("Save_Level", Save_Level);
    Assign("Load_Level", Load_Level);
    Assign("Debug", Debug);
    Assign("Set_Paths", Set_Paths);
    Assign("Open", Open);
    Assign("Close", Close);
    Assign("Use", Use);
    Assign("Read", Read);
    Assign("Write", Write);
    Assign("Exists", Exists);
    Assign("Delete", Delete);
    Assign("Set", Set);
    Assign("Get", Get);
    Assign("Insert", Insert);
    Assign("Update", Update);
    Assign("Find", Find);
    Assign("List", List);
    Assign("Remove", Remove);
    Assign("Begin", Begin);
    Assign("Commit", Commit);
    Assign("Rollback", Rollback);
    Assign("Backup", Backup);
    Assign("Restore", Restore);
    Assign("Export", Export);
    Assign("Import", Import);
    Assign("Save", Save);
    Assign("Load", Load);
    Assign("Execute_SQL", Execute_SQL);
    Assign("Try_SQL", Try_SQL);
    Assign("Open_Memory", Open_Memory);
    Assign("Get_Field", Get_Field);
    Assign("Set_Field", Set_Field);
}

extern "C" void Interpreter(Data_Source *Data)
{
    std::string Command;
    *Data >> Command;

    if(Command == "Create_Level" || Command == "Save_Level" || Command == "Load_Level")
    {
        std::string Name; *Data >> Name;
        if(Command == "Create_Level") {
            if(!Create_Level) throw std::runtime_error("Storage plugin does not support Create_Level");
            Create_Level(Name);
        } else if(Command == "Save_Level") {
            std::string Snapshot; *Data >> Snapshot;
            if(!Save_Level) throw std::runtime_error("Storage plugin does not support Save_Level");
            Save_Level(Name, Snapshot);
        } else {
            if(!Load_Level) throw std::runtime_error("Storage plugin does not support Load_Level");
            Data->Return_Value(Load_Level(Name));
        }
    }
    else if(Command == "Init")
    {
        std::string Name;
        *Data >> Name;
        Init(Name);
    }
    else if(Command == "Get_Field" || Command == "Set_Field")
    {
        std::string Table, Key_Column, Key, Field, Value;
        *Data >> Table;
        *Data >> Key_Column;
        *Data >> Key;
        *Data >> Field;
        if(Command == "Get_Field")
        {
            if(!Get_Field) throw std::runtime_error("Storage plugin does not support Get_Field");
            Data->Return_Value(Get_Field(Table, Key_Column, Key, Field));
        }
        else
        {
            *Data >> Value;
            if(!Set_Field) throw std::runtime_error("Storage plugin does not support Set_Field");
            Set_Field(Table, Key_Column, Key, Field, Value);
        }
    }
    else if(Command == "Open_Memory")
    {
        std::string Name; *Data >> Name;
        if(!Open_Memory) throw std::runtime_error("Storage plugin does not support Open_Memory");
        Open_Memory(Name);
    }
    else if(Command == "Try_SQL")
    {
        std::string SQL; *Data >> SQL;
        if(!Try_SQL) throw std::runtime_error("Storage plugin does not support Try_SQL");
        Data->Return_Value(Try_SQL(SQL));
    }
    else if(Command == "Execute_SQL")
    {
        std::string SQL;
        *Data >> SQL;
        if(!Execute_SQL) throw std::runtime_error("Storage plugin does not support Execute_SQL");
        Execute_SQL(SQL);
    }
    else if(Command == "Debug")
    {
        std::string Value;
        *Data >> Value;
        if(Value == "1" || Value == "On" || Value == "on" || Value == "True" || Value == "true")
        {
            Module_Debug = true;
        }
        else if(Value == "0" || Value == "Off" || Value == "off" || Value == "False" || Value == "false")
        {
            Module_Debug = false;
        }
        if(Debug != nullptr) Debug(Module_Debug);
    }
    else if(Command == "Set_Paths")
    {
        std::string Resources;
        *Data >> Resources;
        Set_Paths(Resources);
    }
    else if(Command == "Open")
    {
        std::string Name;
        *Data >> Name;
        Open(Name);
    }
    else if(Command == "Close")
    {
        std::string Name;
        *Data >> Name;
        Close(Name);
    }
    else if(Command == "Use")
    {
        std::string Name;
        *Data >> Name;
        Use(Name);
    }
    else if(Command == "Read")
    {
        std::string File;
        *Data >> File;
        Read(File);
    }
    else if(Command == "Write")
    {
        std::string Name, File;
        *Data >> Name;
        *Data >> File;
        Write(Name, File);
    }
    else if(Command == "Exists")
    {
        std::string File;
        *Data >> File;
        Exists(File);
    }
    else if(Command == "Delete")
    {
        std::string File;
        *Data >> File;
        Delete(File);
    }
    else if(Command == "Set")
    {
        std::string Key, Value;
        *Data >> Key;
        *Data >> Value;
        Set(Key, Value);
    }
    else if(Command == "Get")
    {
        std::string Key;
        *Data >> Key;
        Get(Key);
    }
    else if(Command == "Insert")
    {
        std::string Collection, Key, Value;
        *Data >> Collection;
        *Data >> Key;
        *Data >> Value;
        Insert(Collection, Key, Value);
    }
    else if(Command == "Update")
    {
        std::string Collection, Key, Value;
        *Data >> Collection;
        *Data >> Key;
        *Data >> Value;
        Update(Collection, Key, Value);
    }
    else if(Command == "Find")
    {
        std::string Collection, Key;
        *Data >> Collection;
        *Data >> Key;
        Find(Collection, Key);
    }
    else if(Command == "List")
    {
        std::string Collection;
        *Data >> Collection;
        List(Collection);
    }
    else if(Command == "Remove")
    {
        std::string Collection, Key;
        *Data >> Collection;
        *Data >> Key;
        Remove(Collection, Key);
    }
    else if(Command == "Begin")
    {
        Begin();
    }
    else if(Command == "Commit")
    {
        Commit();
    }
    else if(Command == "Rollback")
    {
        Rollback();
    }
    else if(Command == "Backup")
    {
        std::string File;
        *Data >> File;
        Backup(File);
    }
    else if(Command == "Restore")
    {
        std::string File;
        *Data >> File;
        Restore(File);
    }
    else if(Command == "Export")
    {
        std::string File;
        *Data >> File;
        Export(File);
    }
    else if(Command == "Import")
    {
        std::string File;
        *Data >> File;
        Import(File);
    }
    else if(Command == "Save")
    {
        std::string Name;
        *Data >> Name;
        Save(Name);
    }
    else if(Command == "Load")
    {
        std::string Name;
        *Data >> Name;
        Load(Name);
    }
    else
    {
        if(Module_Debug) std::cout << "\tError: " << Command << " not found in Storage Dictionary" << std::endl;
    }
}

extern "C" void Shutdown()
{
    Module.Unload();
}
