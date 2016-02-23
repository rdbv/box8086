#include <stdio.h>
#include <iostream>

#include "../Disasm/Disasm.hpp"
#include "../CPU/CPU.hpp"

#include <gtkmm.h>

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

        //test();
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


class RegistersFrame : public Gtk::Frame {

public:
    RegistersFrame() 
        : Gtk::Frame("Registers") {
        


        init_labels();
        init_grid();

        add(grid);

    }

    void init_labels() {
        ax.set_label("AX:0x0000");
        bx.set_label("BX:0x0000");
        cx.set_label("CX:0x0000");
        dx.set_label("DX:0x0000");

        sp.set_label("SP:0x0000");
        bp.set_label("BP:0x0000");
        si.set_label("SI:0x0000");
        di.set_label("DI:0x0000");

        cs.set_label("CS:0x0000");
        ds.set_label("DS:0x0000");
        es.set_label("ES:0x0000");
        ss.set_label("SS:0x0000");

        ip.set_label("IP:0x0000");
        flags.set_label("FLAGS: OV CF ");

    }

    void init_grid() {

        grid.set_border_width(10);
        grid.set_column_spacing(10);

        grid.add(ax);
        grid.add(bx);
        grid.add(cx);
        grid.add(dx);

        grid.attach_next_to(sp, ax, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(bp, bx, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(si, cx, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(di, dx, Gtk::POS_BOTTOM, 1, 1);

        grid.attach_next_to(cs, sp, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(ds, bp, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(es, si, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(ss, di, Gtk::POS_BOTTOM, 1, 1);

        grid.attach_next_to(ip, cs, Gtk::POS_BOTTOM, 1, 1);
        grid.attach_next_to(flags, ds, Gtk::POS_BOTTOM, 1, 1);

    }

    void set_registers(const Registers& regs) {

        char buffer[32] = {0};

        snprintf(buffer, 32, "AX:0x%04x", regs.ax);
        ax.set_label(buffer);
        snprintf(buffer, 32, "BX:0x%04x", regs.bx);
        bx.set_label(buffer);
        snprintf(buffer, 32, "CX:0x%04x", regs.cx);
        cx.set_label(buffer);
        snprintf(buffer, 32, "DX:0x%04x", regs.dx);
        dx.set_label(buffer);

        snprintf(buffer, 32, "SP:0x%04x", regs.sp);
        sp.set_label(buffer);
        snprintf(buffer, 32, "BP:0x%04x", regs.bp);
        bp.set_label(buffer);
        snprintf(buffer, 32, "SI:0x%04x", regs.si);
        si.set_label(buffer);
        snprintf(buffer, 32, "DI:0x%04x", regs.di);
        di.set_label(buffer);
       
        snprintf(buffer, 32, "CS:0x%04x", regs.cs);
        cs.set_label(buffer);
        snprintf(buffer, 32, "DS:0x%04x", regs.ds);
        ds.set_label(buffer);
        snprintf(buffer, 32, "ES:0x%04x", regs.es);
        es.set_label(buffer);
        snprintf(buffer, 32, "SS:0x%04x", regs.ss);
        ss.set_label(buffer);

        snprintf(buffer, 32, "IP:0x%04x", regs.ip);
        ip.set_label(buffer);
        
    }

protected:
    Gtk::Grid grid;
    Gtk::Label ax, bx, cx, dx;
    Gtk::Label sp, bp, si, di;
    Gtk::Label cs, ds, es, ss;
    Gtk::Label ip, flags;

};

class ControlButtonsFrame : public Gtk::Frame {

public:
    ControlButtonsFrame() 
        : Gtk::Frame("Controls"),
          step("_Step", true),
          cont("_Continue", true),
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

    }

    Gtk::Grid grid;
    Gtk::Button step, cont;
    
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

        panel_left.pack_start(regs, Gtk::PACK_SHRINK);
        panel_left.pack_start(buttons_frame, Gtk::PACK_SHRINK);
        panel_left.pack_start(mem_window);

        root_box.pack_start(panel_left, Gtk::PACK_SHRINK);
        root_box.pack_start(code);

        show_all_children();

    }

    void init_signals() {

        buttons_frame.test.signal_clicked().connect(
                sigc::mem_fun(*this, &Box8086_GUI::on_test_clicked));

        buttons_frame.step.signal_clicked().connect(
                sigc::mem_fun(*this, &Box8086_GUI::on_step_clicked));

    }

    void setup() {
        
        mem = cpu.getMemory();
        loadFile("bin/test.bin", &(*mem)[0]);
        disasm.bindMemory(&(*mem)[0]);
        auto v = disasm.disasm(0x0000, 40);

        for(auto i : v)
            code.append_row(i.position, i.instr);

        mem_window.dump_memory(60, 0x0000, &(*mem)[0]);

        regs.set_registers(cpu.getRegisters());

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

    RegistersFrame regs;
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
