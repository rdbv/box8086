#include <stdio.h>
#include <iostream>

#include "../Disasm/Disasm.hpp"
#include "../CPU/CPU.hpp"

#include <gtkmm.h>

/* Ugly code, but i need GUI for do some tests :P */

std::vector<std::string> __regs = {
    "AX", "BX", "CX", "DX",
    "SI", "DI", "SP", "BP",
    "CS", "DS", "SS", "ES",
    "IP", "FLAGS",
};

void loadFile(const std::string& fileName, ubyte* mem) {
   
    unsigned int fileSize = 0;
    FILE* f = fopen(fileName.c_str(), "rb");
    if(f == NULL) {
        printf("Unable to open");
        exit(0); 
    }
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f); 
    rewind(f);
    
    fread(mem, fileSize, 1, f);
}
 
class InstructionColumn : public Gtk::TreeModel::ColumnRecord {
    public:
        InstructionColumn() {
            add(position);
            add(str);
        }

    Gtk::TreeModelColumn<std::string> position;
    Gtk::TreeModelColumn<std::string> str;

};

class MemoryColumn : public Gtk::TreeModel::ColumnRecord {
public:
    MemoryColumn() {
        add(position);
        for(unsigned int i=0;i<0xf;++i)
            add(bytes[i]);

    }
    
    Gtk::TreeModelColumn<std::string> position;
    Gtk::TreeModelColumn<std::string> bytes[0xf];

};

class CodeViewWindow : public Gtk::ScrolledWindow {

public:
    CodeViewWindow() {
        add(tv);
        set_border_width(10);

        init_view();
    }

    void append_row(unsigned int position, std::string str) {

        char buffer[32] = {0};
        snprintf(buffer, 32, "0x%x", position);
        
        Gtk::TreeModel::Row row = *(ref_tree_model->append());
        row[columns.position] = std::string(buffer);
        row[columns.str] = str;

    }

    void init_view() {

        ref_tree_model = Gtk::ListStore::create(columns);
        tv.set_model(ref_tree_model);
    
        tv.append_column("Address", columns.position);
        tv.append_column("Instruction", columns.str);


    }

    void delete_all_rows() {
        ref_tree_model->clear();
    }

    Gtk::TreeView tv;

    InstructionColumn columns;
    Glib::RefPtr<Gtk::ListStore> ref_tree_model;


};

class MemoryViewWindow : public Gtk::ScrolledWindow {

public:
    MemoryViewWindow() {
        add(tv);

        init_view();
    }

    void dump_memory(unsigned int count, unsigned int start_address, ubyte* mem) {

        ref_tree_model->clear();

        for(unsigned int i=0;i<count;++i) {
           
            Gtk::TreeModel::Row row = *(ref_tree_model->append());

            char buffer_pos[32] = {0};
            snprintf(buffer_pos, 32, "%04x", start_address+i*0xf);
            row[columns.position] = buffer_pos;

            for(unsigned int byte_no = 0; byte_no < 0xf; ++byte_no) {
                char buffer[8] = {0};
                snprintf(buffer, 8, "%x", mem[(start_address+i*0xf)+byte_no]);
                row[columns.bytes[byte_no]] = buffer;
            }
        }
    }

    void init_view() {
        
        ref_tree_model = Gtk::ListStore::create(columns);
        tv.set_model(ref_tree_model);

        tv.append_column("Address", columns.position);
        for(int i=0;i<0xf;++i) {
            char buffer[32] = {0};
            snprintf(buffer, 32, "%x", i);
            tv.append_column(buffer, columns.bytes[i]);
        }

    }

    Gtk::TreeView tv;

    MemoryColumn columns;
    Glib::RefPtr<Gtk::ListStore> ref_tree_model;
};


class RegisterColumn : public Gtk::TreeModel::ColumnRecord {
    public:
        RegisterColumn() {
            add(register_name);
            add(str_value);
        }

    Gtk::TreeModelColumn<std::string> register_name;
    Gtk::TreeModelColumn<std::string> str_value;

};

class RegistersViewFrame : public Gtk::ScrolledWindow {
    
public:
    RegistersViewFrame() {
        add(sw);
        sw.add(tv);
        init_view();

    }
    
    void init_view() {
        ref_tree_model = Gtk::ListStore::create(columns);
        tv.set_model(ref_tree_model);

        tv.append_column("Register", columns.register_name);
        tv.append_column("Value (Hexadecimal)", columns.str_value);

        for(unsigned int i=0;i<__regs.size();++i) {
            Gtk::TreeModel::Row row = *(ref_tree_model->append());
            row[columns.register_name] = __regs[i];
            if(i != __regs.size()-1)
                row[columns.str_value] = "0x0000";
        }
    }

    void set_registers(const Registers& regs) {
        
        char buffer[32] = {0};

        typedef Gtk::TreeModel::Children t_ch;
        t_ch ch = ref_tree_model->children();

        t_ch::iterator it = ch.begin();

        Gtk::TreeModel::Row row = *it;

        snprintf(buffer, 32, "0x%04x", regs.ax);
        row[columns.str_value] = buffer;

        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.bx);
        row[columns.str_value] = buffer;
      
        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.cx);
        row[columns.str_value] = buffer;

        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.dx);
        row[columns.str_value] = buffer;

        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.si);
        row[columns.str_value] = buffer;
      
        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.di);
        row[columns.str_value] = buffer;

        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.sp);
        row[columns.str_value] = buffer;
      
        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.bp);
        row[columns.str_value] = buffer;
        
        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.cs);
        row[columns.str_value] = buffer;
      
        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.ds);
        row[columns.str_value] = buffer;

        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.ss);
        row[columns.str_value] = buffer;
      
        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.es);
        row[columns.str_value] = buffer;

        it++;
        row = *it;
        snprintf(buffer, 32, "0x%04x", regs.ip);
        row[columns.str_value] = buffer;
        
        it++;
        row = *it; 
        std::string flags_buf;
        if(regs.flags[CF]) flags_buf += "CF ";
        if(regs.flags[PF]) flags_buf += "PF ";
        if(regs.flags[AF]) flags_buf += "AF ";
        if(regs.flags[ZF]) flags_buf += "ZF ";
        if(regs.flags[SF]) flags_buf += "SF ";
        if(regs.flags[TF]) flags_buf += "TF ";
        if(regs.flags[IF]) flags_buf += "IF ";
        if(regs.flags[DF]) flags_buf += "DF ";
        if(regs.flags[OF]) flags_buf += "OF ";
        row[columns.str_value] = flags_buf;
    }

    Gtk::ScrolledWindow sw;
    Gtk::TreeView tv;
    RegisterColumn columns;
    Glib::RefPtr<Gtk::ListStore> ref_tree_model;
};

class ControlButtonsFrame : public Gtk::Frame {

public:
    ControlButtonsFrame() 
        : Gtk::Frame("Controls"),
          step("_Step", true),
          cont("_Continue", true),
          exit("_Exit", true),
          test("_Test", true)
    {

        init_labels();
        init_grid();

        add(grid);
    }

    void init_labels() {

    }

    void init_grid() {

        grid.set_border_width(10);
        grid.set_column_spacing(10);

        grid.add(step);
        grid.add(cont);
        grid.add(test);
        grid.add(exit);

    }

    Gtk::Grid grid;

    Gtk::Button step, cont;
    Gtk::Button exit;

    Gtk::Button test;

};

class Box8086_GUI : public Gtk::Window {

public:
    Box8086_GUI() {
        init_window();
    }
    virtual ~Box8086_GUI() {
    }

private:

    void init_window() {
    
        set_title("box8086 debugger");
        set_border_width(10);
        set_size_request(900, 900);

        init_signals();
        init_layout();

        setup();

    }

    void init_layout() {

        add(root_box);


        panel_left.set_orientation(Gtk::Orientation::ORIENTATION_VERTICAL);
        panel_left.set_spacing(10);

        panel_left.pack_start(regs);
        panel_left.pack_start(mem_window);
        panel_left.pack_start(buttons_frame);

        root_box.pack_start(panel_left, Gtk::PACK_SHRINK);
        root_box.pack_start(code);

        show_all_children();

    }

    void init_signals() {

        buttons_frame.test.signal_clicked().connect(
                sigc::mem_fun(*this, &Box8086_GUI::on_test_clicked));

        buttons_frame.step.signal_clicked().connect(
                sigc::mem_fun(*this, &Box8086_GUI::on_step_clicked));

        buttons_frame.exit.signal_clicked().connect(
                sigc::mem_fun(*this, &Box8086_GUI::on_exit_clicked));

    }

    void setup() {
        
        mem = cpu.getMemory();
        loadFile("bin/test.bin", &(*mem)[0]);
        disasm.bindMemory(&(*mem)[0]);
        auto v = disasm.disasm(0x0000, 40);

        for(auto i : v)
            code.append_row(i.position, i.instr);

        mem_window.dump_memory(60, 0x0000, &(*mem)[0]);

       // regs.set_registers(cpu.getRegisters());

    }

protected:

    void on_test_clicked() {
        std::cout << "Test!" << std::endl;
   
        code.delete_all_rows();
    }

    void on_step_clicked() {

        cpu.step();
        regs.set_registers(cpu.getRegisters());
        mem_window.dump_memory(60, 0x0000, &(*mem)[0]);

    }


    void on_exit_clicked() {
        exit(0);
    }

    RegistersViewFrame regs;
    CodeViewWindow code;
    MemoryViewWindow mem_window; 
    ControlButtonsFrame buttons_frame;

    Gtk::Box root_box;
    Gtk::Box panel_left;

   
    Disasm disasm;
    Memory* mem; 
    CPU cpu;


}; 

int main(int argc, char** argv) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Box8086_GUI gui; 
    return app->run(gui);
}
