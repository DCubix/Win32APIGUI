#include <iostream>

#include "gui.hpp"

class Calculator : public gui::Window {
public:

	void onCreate(gui::Manager& man) override {
		setTitle(L"Calculator");

		gui::Container& rootPane = root();
		rootPane.flow = gui::Flow::Vertical;

		gui::Edit& display = man.create<gui::Edit>(rootPane.id());
		display.size.height = 40;
		display.flex = 0;
		display.readOnly(true);
		display.textAlignment(gui::Alignment::Right);
		display.fontSize(32);
		display.text(L"0");

		wchar_t keypad[] = {
			'7', '8', '9', '+',
			'4', '5', '6', '-',
			'1', '2', '3', L'�',
			'.', '0', '=', L'�'
		};

		gui::Container& funcRow = man.create<gui::Container>(rootPane.id(), gui::Size{ 800, 50 }, gui::Flow::Horizontal);
		funcRow.flex = 0;
		funcRow.border = 0;

		{
			gui::Button& btn = man.create<gui::Button>(funcRow.id(), L"C");
			btn.flex = 1;
			btn.fontSize(28);
			btn.onPressed = [&]() {
				display.text(L"0");
			};
		}

		{
			gui::Button& btn = man.create<gui::Button>(funcRow.id(), L"CE");
			btn.flex = 1;
			btn.fontSize(28);
			btn.onPressed = [&]() {
				display.text(L"0");
				op = '\0';
				n1 = ans = 0.0f;
				solved = false;
			};
		}

		for (int i = 0; i < 4; i++) {
			gui::Container& row = man.create<gui::Container>(rootPane.id(), gui::Size{ 800, 50 }, gui::Flow::Horizontal);
			row.flex = 0;
			row.border = 0;
			for (int j = 0; j < 4; j++) {
				wchar_t cop = keypad[j + i * 4];
				gui::Button& numBtn = man.create<gui::Button>(row.id(), gui::String(1, cop));
				numBtn.fontSize(28);
				numBtn.onPressed = [&]() {
					TCHAR chr = numBtn.text()[0];
					switch (chr) {
						case '+':
						case '-':
						case L'�':
						case L'�':
							if (op == '\0') n1 = std::stof(display.text());
							else {
								if (!solved) ans = std::stof(display.text());
								else {
									n1 = std::stof(display.text());
									solved = false;
								}
							}

							op = chr;
							display.text(L"0");
							break;
						case '.':
							display.text(display.text() + gui::String(1, '.'));
							break;
						case '=': {
							ans = std::stof(display.text());
							bool divisionByZero = false;
							switch (op) {
								case '+': ans += n1; break;
								case '-': ans -= n1; break;
								case L'�': ans *= n1; break;
								case L'�': if (ans > 0.0f) { ans = n1 / ans; } else { divisionByZero = true; } break;
							}
							if (!divisionByZero) {
								n1 = ans;
								display.text(std::to_wstring(ans));
								solved = true;
							} else display.text(L"Division by Zero!");
						} break;
						default:
							if (solved) {
								display.text(L"0");
								op = '\0';
								solved = false;
							}
							if (display.text() == L"0") {
								display.text(L"");
							}
							gui::String tmp = display.text();
							display.text(tmp + gui::String(1, chr));
							break;
					}
				};
			}
		}

		man.create<gui::Label>(rootPane.id(), L"Made by Diego");

		resize(300, 355);
	}

	wchar_t op{ '\0' };
	float n1{ 0.0f }, ans{ 0.0f };
	bool solved{ false };
};

class OpenGLTriangle : public gui::GLView {
public:
	void onPaint() override {
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);

		glEnd();
	}
};

class WidgetsTest : public gui::Window {
public:
	void onCreate(gui::Manager& man) override {
		setTitle(L"Widgets");

		gui::Container& rootPane = root();
		rootPane.flow = gui::Flow::Vertical;

		OpenGLTriangle& glv = man.create<OpenGLTriangle>(rootPane.id());

		gui::Container& buttonPane = man.create<gui::Container>(rootPane.id());
		buttonPane.flex = 0;
		buttonPane.size.height = 40;

		gui::Button& btn = man.create<gui::Button>(buttonPane.id(), L"Button");
		btn.size.width = 120;
		btn.flex = 0;
	}
};

class RichEditTest : public gui::Window {
public:
	void onCreate(gui::Manager& man) override {
		root().flow = gui::Flow::Vertical;

		gui::CodeEdit& red = man.create<gui::CodeEdit>(root().id());
		red.flex = 1;
		red.background = gui::Color{ 40, 40, 40, 255 };
		red.foreground = gui::Color{ 240, 240, 240, 255 };

		gui::HighlightingRule kw1Rule = {
			.style = {
				.background = red.background,
				.foreground = gui::Color{ 20, 177, 217, 255 },
				.bold = true
			},
			.pattern = L"\\b(if|else|elif|pass|not|or|and|in|for|while|with|as|try|except|finally|yield|return)\\b"
		};
		red.highlightingRules.push_back(kw1Rule);

		gui::HighlightingRule kw2Rule = {
			.style = {
				.background = red.background,
				.foreground = gui::Color{ 217, 177, 20, 255 },
				.bold = true
			},
			.pattern = L"\\b(class|def|self)\\b"
		};
		red.highlightingRules.push_back(kw2Rule);

		gui::HighlightingRule kw3Rule = {
			.style = {
				.background = red.background,
				.foreground = gui::Color{ 20, 217, 177, 255 },
				.bold = true
			},
			.pattern = L"\\b(True|False|None)\\b"
		};
		red.highlightingRules.push_back(kw3Rule);

		gui::Container& cnt = man.create<gui::Container>(root().id(), gui::Size{ 0, 32 });
		cnt.flex = 0;

		gui::Button& btn = man.create<gui::Button>(cnt.id(), L"Colorize!");
		btn.size.width = 120;
		btn.size.height = 32;
		btn.flex = 0;

		btn.onPressed = [&]() {
			red.rtf(LR"({\rtf1\ansi{\fonttbl\f0\fswiss Helvetica;}\f0\pard
			This is some{ \b bold } text.\par
	})");
		};

		resize(400, 400);
	}
};

class SplitterTest : public gui::Window {
public:
	void onCreate(gui::Manager& man) override {
		root().flow = gui::Flow::Vertical;

		gui::Splitter& spl = man.create<gui::Splitter>(root().id());

		resize(800, 600);
	}
};

int main(int argc, char** argv) {
	//Calculator cal1{};
	//cal1.show();

	//WidgetsTest wt{};
	//wt.show();

	RichEditTest win{};
	win.show();

	//SplitterTest st{};
	//st.show();

	gui::Window::mainLoop();

	return 0;
}
