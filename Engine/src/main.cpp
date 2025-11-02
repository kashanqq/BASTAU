///Standard Libraty Includes
#include <string>
#include <thread>
#include <iostream>
#include <math.h>
#include <ctime>
#include <regex>
#include <chrono>
#include <algorithm>

///Dependencies Library Includes
#include <IrrIMGUI/IncludeIrrlicht.h>
#include <IrrIMGUI/IncludeIMGUI.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <IrrIMGUI/IrrIMGUIDebug.h>
#include <IconsFontAwesome5.h>

///Custom Classes Headers
#include "CGridSceneNode.h"
#include "NodeTree.h"
#include "IO.h"
#include "LogApp.h"
#include "Constants.h"
#include "InterfaceStyle.h"
#include "Initializer.h"
#include "Functionality.h"
#include "FileParser.h"
#include "jagpdf\api.h"

///NameSpacesx	

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;	
using namespace IrrIMGUI;
using namespace CONSTANTS;
using namespace jag;
///Global variables
///Irrlicht 
IrrlichtDevice			*device = 0;
IVideoDriver			*driver = 0;
ISceneManager			*smgr = 0;
ICameraSceneNode		*camera = 0;
ISceneCollisionManager  *CollMan = 0;
///Program
bool TOOLS_S[MAX_SPECTATOR_TOOLS];
bool TOOLS_E[MAX_EDITOR_TOOLS];
bool WINDOWS[MAX_WINDOWS];
bool WINDOWS_COLLAPSED[MAX_WINDOWS];
bool VIEWMODES[MAX_USERMODES];
bool CAMERAMODES[MAX_CAMERA_MODES];
bool isBuilt = true;
bool dataRead = true;
bool isSecond = false;
bool IsMouseMoved = false;
bool isNodeHovered = false;
bool logWindowCollapsed = false;
bool update = false;
float r = 2000.f, theta = 1.8f, phi = -1.15f;
float pointX = 0.f;
float diffX = 0.f, diffY = 0.f;
float rotation = 0.0f;
int Y = 830;
///Functions
void toolSelectEditor(ToolEditor tool);
void toolSelectSpectator(ToolsSpectator tool);
void switchViewMode(UserMode mode);
bool try_stoi(int & i, const std::string & s);
bool TabLabels(int numTabs, const char** tabLabels, int& selectedIndex, const char** tabLabelTooltips = NULL, bool wrapMode = true, int *pOptionalHoveredIndex = NULL, int* pOptionalItemOrdering = NULL, bool allowTabReorder = true, bool allowTabClosingThroughMMB = true, int *pOptionalClosedTabIndex = NULL, int *pOptionalClosedTabIndexInsideItemOrdering = NULL)
{
	ImGuiStyle& style = ImGui::GetStyle();

	const ImVec2 itemSpacing = style.ItemSpacing;
	const ImVec4 color = style.Colors[ImGuiCol_Button];
	const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
	const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
	const ImVec4 colorText = style.Colors[ImGuiCol_Text];
	style.ItemSpacing.x = 1;
	style.ItemSpacing.y = 1;
	const ImVec4 colorSelectedTab(color.x, color.y, color.z, color.w*0.5f);
	const ImVec4 colorSelectedTabHovered(colorHover.x, colorHover.y, colorHover.z, colorHover.w*0.5f);
	const ImVec4 colorSelectedTabText(colorText.x*0.8f, colorText.y*0.8f, colorText.z*0.6f, colorText.w*0.8f);
	//ImGui::ClampColor(colorSelectedTabText);

	if (numTabs>0 && (selectedIndex<0 || selectedIndex >= numTabs)) {
		if (!pOptionalItemOrdering)  selectedIndex = 0;
		else selectedIndex = -1;
	}
	if (pOptionalHoveredIndex) *pOptionalHoveredIndex = -1;
	if (pOptionalClosedTabIndex) *pOptionalClosedTabIndex = -1;
	if (pOptionalClosedTabIndexInsideItemOrdering) *pOptionalClosedTabIndexInsideItemOrdering = -1;

	float windowWidth = 0.f, sumX = 0.f;
	if (wrapMode) windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY()>0 ? style.ScrollbarSize : 0.f);

	static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;   // These are indices inside pOptionalItemOrdering
	static ImVec2 draggingTabSize(0, 0);
	static ImVec2 draggingTabOffset(0, 0);

	const bool isMMBreleased = ImGui::IsMouseReleased(2);
	const bool isMouseDragging = ImGui::IsMouseDragging(0, 2.f);
	int justClosedTabIndex = -1, newSelectedIndex = selectedIndex;


	bool selection_changed = false; bool noButtonDrawn = true;
	for (int j = 0, i; j < numTabs; j++)
	{
		i = pOptionalItemOrdering ? pOptionalItemOrdering[j] : j;
		if (i == -1) continue;

		if (!wrapMode) { if (!noButtonDrawn) ImGui::SameLine(); }
		else if (sumX > 0.f) {
			sumX += style.ItemSpacing.x;   // Maybe we can skip it if we use SameLine(0,0) below
			sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f*style.FramePadding.x;
			if (sumX>windowWidth) sumX = 0.f;
			else ImGui::SameLine();
		}

		if (i == selectedIndex) {
			// Push the style
			style.Colors[ImGuiCol_Button] = colorSelectedTab;
			style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
			style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
			style.Colors[ImGuiCol_Text] = colorSelectedTabText;
		}
		// Draw the button
		ImGui::PushID(i);   // otherwise two tabs with the same name would clash.
		if (ImGui::Button(tabLabels[i])) { selection_changed = (selectedIndex != i); newSelectedIndex = i; }
		ImGui::PopID();
		if (i == selectedIndex) {
			// Reset the style
			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
			style.Colors[ImGuiCol_Text] = colorText;
		}
		noButtonDrawn = false;

		if (wrapMode) {
			if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
		}
		else if (isMouseDragging && allowTabReorder && pOptionalItemOrdering) {
			// We still need sumX
			if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line
			else sumX += style.ItemSpacing.x + ImGui::GetItemRectSize().x;

		}

		if (ImGui::IsItemHoveredRect()) {
			if (pOptionalHoveredIndex) *pOptionalHoveredIndex = i;
			if (tabLabelTooltips && tabLabelTooltips[i] && strlen(tabLabelTooltips[i])>0)  ImGui::SetTooltip("%s", tabLabelTooltips[i]);

			if (pOptionalItemOrdering) {
				if (allowTabReorder) {
					if (isMouseDragging) {
						if (draggingTabIndex == -1) {
							draggingTabIndex = j;
							draggingTabSize = ImGui::GetItemRectSize();
							const ImVec2& mp = ImGui::GetIO().MousePos;
							const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
							draggingTabOffset = ImVec2(
								mp.x + draggingTabSize.x*0.5f - sumX + ImGui::GetScrollX(),
								mp.y + draggingTabSize.y*0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
							);

						}
					}
					else if (draggingTabIndex >= 0 && draggingTabIndex<numTabs && draggingTabIndex != j) {
						draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
					}
				}
				if (allowTabClosingThroughMMB) {
					if (isMMBreleased) {
						justClosedTabIndex = i;
						if (pOptionalClosedTabIndex) *pOptionalClosedTabIndex = i;
						if (pOptionalClosedTabIndexInsideItemOrdering) *pOptionalClosedTabIndexInsideItemOrdering = j;
						pOptionalItemOrdering[j] = -1;
					}
				}
			}
		}

	}

	selectedIndex = newSelectedIndex;

	// Draw tab label while mouse drags it
	if (draggingTabIndex >= 0 && draggingTabIndex<numTabs) {
		const ImVec2& mp = ImGui::GetIO().MousePos;
		const ImVec2 wp = ImGui::GetWindowPos();
		ImVec2 start(wp.x + mp.x - draggingTabOffset.x - draggingTabSize.x*0.5f, wp.y + mp.y - draggingTabOffset.y - draggingTabSize.y*0.5f);
		const ImVec2 end(start.x + draggingTabSize.x, start.y + draggingTabSize.y);
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const float draggedBtnAlpha = 0.65f;
		const ImVec4& btnColor = style.Colors[ImGuiCol_Button];
		drawList->AddRectFilled(start, end, ImColor(btnColor.x, btnColor.y, btnColor.z, btnColor.w*draggedBtnAlpha), style.FrameRounding);
		start.x += style.FramePadding.x; start.y += style.FramePadding.y;
		const ImVec4& txtColor = style.Colors[ImGuiCol_Text];
		drawList->AddText(start, ImColor(txtColor.x, txtColor.y, txtColor.z, txtColor.w*draggedBtnAlpha), tabLabels[pOptionalItemOrdering[draggingTabIndex]]);

		ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
	}

	// Drop tab label
	if (draggingTabTargetIndex != -1) {
		// swap draggingTabIndex and draggingTabTargetIndex in pOptionalItemOrdering
		const int tmp = pOptionalItemOrdering[draggingTabTargetIndex];
		pOptionalItemOrdering[draggingTabTargetIndex] = pOptionalItemOrdering[draggingTabIndex];
		pOptionalItemOrdering[draggingTabIndex] = tmp;
		//fprintf(stderr,"%d %d\n",draggingTabIndex,draggingTabTargetIndex);
		draggingTabTargetIndex = draggingTabIndex = -1;
	}

	// Reset draggingTabIndex if necessary
	if (!isMouseDragging) draggingTabIndex = -1;

	// Change selected tab when user closes the selected tab
	if (selectedIndex == justClosedTabIndex && selectedIndex >= 0) {
		selectedIndex = -1;
		for (int j = 0, i; j < numTabs; j++) {
			i = pOptionalItemOrdering ? pOptionalItemOrdering[j] : j;
			if (i == -1) continue;
			selectedIndex = i;
			break;
		}
	}

	// Restore the style
	style.Colors[ImGuiCol_Button] = color;
	style.Colors[ImGuiCol_ButtonActive] = colorActive;
	style.Colors[ImGuiCol_ButtonHovered] = colorHover;
	style.Colors[ImGuiCol_Text] = colorText;
	style.ItemSpacing = itemSpacing;

	return selection_changed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std::chrono_literals;

#include <windows.h>
///Main Function
int main()
{
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

	std::cout << "Debug console started" << std::endl;

	/*FreeConsole();*/
	std::vector<std::string> searchBuff;
	ISceneNode* lastHovered = 0;
	ISceneNode* lastSelected = 0;
	ISceneNode* hovered = 0;
	ISceneNode* selected = 0;

	Node* selNode = nullptr;
	LogApp log;
	IO fileIO;
	SMaterial lineMaterial;

	std::string filterString;
	//FIX FOR MULTI LANG
	static char searchBuffer[64] = "";
	static char filterBuffer[64] = "";
	irr::core::recti selectBox;
	irr::core::list<Node*> selectedNodes;
	irr::core::list<Node*> filteredNodes;

	vector2di firstCorner, secondCorner;
	bool firstCornerSet = false;

	std::vector<std::vector<std::string>> valueMatrix;
	std::vector<std::vector<std::string>> summaryData;
	summaryData.resize(6);
	for (auto &i : summaryData)
	{
		i.resize(2);
	}
	const std::string summtags[7] = { "Units:", "Staff:", "Budget:", "Expenses:" ,"Income:" ,"Loss:" };
	for (int i = 0; i < 6; i++)
	{
		summaryData[i][0] = summtags[i];
	}

	VIEWMODES[UserMode::USERMODE_EDITOR] = true;
	TOOLS_S[ToolsSpectator::TS_SELECTOR] = true;
	TOOLS_E[ToolEditor::TE_SELECTOR] = true;
	CAMERAMODES[CameraMode::CAMERA_FREE] = true;

	class MyEventReceiver : public IrrIMGUI::CIMGUIEventReceiver
	{
	public:
		bool mKeyW = false;
		bool mKeyA = false;
		bool mKeyS = false;
		bool mKeyD = false;
		bool mKeyQ = false;
		bool mKeyE = false;
		bool mRMB = false;

		virtual bool OnEvent(const irr::SEvent& event)
		{
			IrrIMGUI::CIMGUIEventReceiver::OnEvent(event);

			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			{
				if (event.KeyInput.Key == irr::KEY_KEY_W)
					mKeyW = event.KeyInput.PressedDown;
				else if (event.KeyInput.Key == irr::KEY_KEY_A)
					mKeyA = event.KeyInput.PressedDown;
				else if (event.KeyInput.Key == irr::KEY_KEY_S)
					mKeyS = event.KeyInput.PressedDown;
				else if (event.KeyInput.Key == irr::KEY_KEY_D)
					mKeyD = event.KeyInput.PressedDown;
				else if (event.KeyInput.Key == irr::KEY_KEY_Q)
					mKeyQ = event.KeyInput.PressedDown;
				else if (event.KeyInput.Key == irr::KEY_KEY_E)
					mKeyE = event.KeyInput.PressedDown;
			}
			else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			{
				if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
					mRMB = true;
				else if (event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP)
					mRMB = false;
			}

			return false;
		}
	};

	MyEventReceiver EventReceiver;

	device = createDeviceEx(Initializer::irrlichtDriverParams(EventReceiver));
	if (!device)
	{
		return 1;
	}
	device->setWindowCaption(L"Bastau");
	device->setResizable(true);

	SIMGUISettings pSettings;
	pSettings.mIsGUIMouseCursorEnabled = false;

	IIMGUIHandle* const pGUI = createIMGUI(device, &EventReceiver, &pSettings);

	///Interface color theme
	InterfaceStyle::setThemeBlue(ImGui::GetStyle());

	///Fonts

	ImFontConfig f_cfg;
	f_cfg.OversampleH = 1;
	f_cfg.OversampleV = 1;
	f_cfg.PixelSnapH = 1;
	static const ImWchar range[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};

	ImFont *consolas12 = pGUI->addFontFromFileTTF("fonts/Consolas.ttf", 12.0f, &f_cfg, range);
	ImFont *consolas16 = pGUI->addFontFromFileTTF("fonts/Consolas.ttf", 16.0f, &f_cfg, range);
	static const ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig iconsConfig;
	iconsConfig.MergeMode = true;
	iconsConfig.PixelSnapH = true;

	pGUI->addFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 12.f, &iconsConfig, iconsRanges);
	pGUI->compileFonts();
	///Fonts end

	driver = device->getVideoDriver();
	driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
	smgr = device->getSceneManager();
	CollMan = smgr->getSceneCollisionManager();

	///Node textures
	ITexture * selectedNodeTexture = Functionality::generatePlainColorTexture(SColor(255, 255, 166, 22), driver);
	ITexture * defaultNodeTexture = Functionality::generatePlainColorTexture(SColor(255, 22, 146, 255), driver);
	ITexture * highlightDefaultNodeTexture = Functionality::generatePlainColorTexture(SColor(255, 139, 201, 255), driver);
	ITexture * highlightSelectedNodeTexture = Functionality::generatePlainColorTexture(SColor(255, 255, 201, 139), driver);
	///Node textures end

	///Static world grid
	CGridSceneNode* grid = new CGridSceneNode(smgr->getRootSceneNode(), smgr);
	if (grid)
	{
		grid->setIsDebugObject(true);
		grid->setPosition(vector3df(0, -3000, 0));
		grid->setScale(vector3df(20, 20, 20));
	}
	grid->drop();
	///Static world grid end



	camera = smgr->addCameraSceneNode();
	camera->setIsDebugObject(true);
	scene::ISceneNode* cameraTarget;



	if (camera)
	{
		camera->setPosition(vector3df(-900, 100, r));
		camera->setAspectRatio(16.0f / 9.0f);
	}

	int lastFPS = -1;
	u32 then = device->getTimer()->getTime();

	position2di cursorPosition;
	position2di cameraTargetObjectPosition;

	scene::ISceneNode* center = smgr->addEmptySceneNode();
	if (center)
	{
		center->setPosition(core::vector3df(0, 0, 0));
	}

	cameraTarget = center;
	camera->setTarget(cameraTarget->getAbsolutePosition());

	camera->setFarValue(40000.f);

	const int size = 1000;
	Node aaa[size];
	Node** nodes = new Node*[size];
	fileIO.readFromFile("nodes.txt", size, nodes, aaa, size);

	///LightNode above camera
	ILightSceneNode*  pLight = device->getSceneManager()->addLightSceneNode();
	SLight & l = pLight->getLightData();
	l.Type = ELT_POINT;
	pLight->setIsDebugObject(true);
	pLight->setPosition(vector3df(40, 40, 20));
	pLight->setParent(camera);
	smgr->setAmbientLight(SColor(255, 180, 180, 180));
	///LightNode above camera end

	NodeTree tree;
	tree.setNodeArray(nodes, size);
	tree.findAndSetRoot(nodes, size);
	tree.buildTreeLogical(tree.getRoot());
	core::vector3df startPos(0, 0, 0);
	tree.setNodeInfo("nodesD.txt");
	tree.buildTreeGraphicalHemisphere(tree.getRoot(), driver, smgr, startPos, startPos);

	int option = 0;

	bool isFiltered = false;

	bool anim = false;
	core::vector3df DistVec = core::vector3df(0, 0, 0);

	core::vector3df cameraTargetPosition = cameraTarget->getAbsolutePosition();
	bool tIsActive = false;




	///MAIN LOOP
	while (device->run())
	{
		irr::core::dimension2du screenSize = driver->getScreenSize();
		irr::core::dimension2df interfaceButton = irr::core::dimension2df((float)(screenSize.Width / 7.2f), (float)(screenSize.Height / 12.6f));
		float spacing = (screenSize.Width - interfaceButton.Width * 6.f) / 7.f;
		///FPS COUNTER
		const u32 now = device->getTimer()->getTime();
		const float frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
		///FPS COUNTER END

		///IRRLICHT INIT
		driver->beginScene(true, true, video::SColor(255, 0, 0, (irr::u32)(0.062 * 255)));
		smgr->drawAll();
		///IRRLICHT INIT END

		tree.render(tree.getRoot(), driver);

		core::vector3df cameraPosition = camera->getAbsolutePosition();
		core::vector3df cameraRotation = camera->getRotation();
		float distance = cameraPosition.getDistanceFrom(cameraTargetPosition);


		if (ImGui::IsKeyDown(37)) {
			phi += 0.2f * frameDeltaTime;
		}
		if (ImGui::IsKeyDown(39)) {
			phi -= 0.2f * frameDeltaTime;
		}
		if (ImGui::IsKeyDown(38)) {
			theta += 0.1f * frameDeltaTime;
		}
		if (ImGui::IsKeyDown(40)) {
			theta -= 0.1f * frameDeltaTime;
		}
		if (EventReceiver.mKeyZPressed) {
			std::cout << r << std::endl;
			r -= 500.f * frameDeltaTime;
		}

		///InterFace 
		pGUI->startGUI();
		ImGui::PushFont(consolas12);
		///Mouse move trigger
		if ((cursorPosition.X != (s32)ImGui::GetMousePos().x) || (cursorPosition.Y != (s32)ImGui::GetMousePos().y))
		{
			IsMouseMoved = true;
		}
		else
		{
			IsMouseMoved = false;
		}
		

		
		///Top MainMenuBar
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.f, 5.f));
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Mode"))
				{
					if (ImGui::MenuItem(ICON_FA_BINOCULARS" Spectator", NULL, VIEWMODES[UserMode::USERMODE_SPECTATOR]))
					{
						if (VIEWMODES[UserMode::USERMODE_SPECTATOR] == 0)
						{
							switchViewMode(UserMode::USERMODE_SPECTATOR);
						}
					}
					if (ImGui::MenuItem(ICON_FA_EDIT" Editor", NULL, VIEWMODES[UserMode::USERMODE_EDITOR]))
					{
						if (VIEWMODES[UserMode::USERMODE_EDITOR] == 0)
						{
							switchViewMode(UserMode::USERMODE_EDITOR);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("ToolBox", NULL, WINDOWS[Window::WINDOW_TOOLS]))
				{
					if (WINDOWS[Window::WINDOW_TOOLS] == 0) WINDOWS[Window::WINDOW_TOOLS] = 1;
					else WINDOWS[Window::WINDOW_TOOLS] = 0;
				}
				ImGui::Separator();
				if (ImGui::BeginMenu("Camera Mode"))
				{
					if (ImGui::MenuItem(ICON_FA_SYNC_ALT" Orbit Camera", NULL, CAMERAMODES[CameraMode::CAMERA_ORBIT]))
					{
						if (CAMERAMODES[CameraMode::CAMERA_ORBIT] == 0)
						{
							CAMERAMODES[CameraMode::CAMERA_ORBIT] = 1;
							CAMERAMODES[CameraMode::CAMERA_FREE] = 0;
						}
					}
					if (ImGui::MenuItem(ICON_FA_ARROWS_ALT" Free Camera (FPS)", NULL, CAMERAMODES[CameraMode::CAMERA_FREE]))
					{
						if (CAMERAMODES[CameraMode::CAMERA_FREE] == 0)
						{
							CAMERAMODES[CameraMode::CAMERA_FREE] = 1;
							CAMERAMODES[CameraMode::CAMERA_ORBIT] = 0;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Data"))
			{
				if (ImGui::BeginMenu("Windows"))
				{
					if (ImGui::MenuItem("Single Unit", NULL, WINDOWS[Window::WINDOW_UNIT_DATA]))
					{
						if (WINDOWS[Window::WINDOW_UNIT_DATA] == 0) WINDOWS[Window::WINDOW_UNIT_DATA] = 1;
						else WINDOWS[Window::WINDOW_UNIT_DATA] = 0;
					}
					if (ImGui::MenuItem("Multi Unit", NULL, WINDOWS[Window::WINDOW_MULTI_UNIT_DATA]))
					{
						if (WINDOWS[Window::WINDOW_MULTI_UNIT_DATA] == 0) WINDOWS[Window::WINDOW_MULTI_UNIT_DATA] = 1;
						else WINDOWS[Window::WINDOW_MULTI_UNIT_DATA] = 0;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Log", NULL, WINDOWS[Window::WINDOW_LOG]))
				{
					if (WINDOWS[Window::WINDOW_LOG] == 0) WINDOWS[Window::WINDOW_LOG] = 1;
					else WINDOWS[Window::WINDOW_LOG] = 0;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
			ImGui::PopStyleVar();
		}
		///Top MainMenuBar end

		///Single unit data window
		if (WINDOWS[Window::WINDOW_UNIT_DATA])
		{
			ImGui::SetNextWindowSize(ImVec2(375.f, 210.f));
			ImGui::Begin("Unit Data", &WINDOWS[WINDOW_UNIT_DATA], ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize);

			if (selNode)
			{
				ImGui::Columns(2, "singleUnitData");

				const char* tags[11] = { "Unit ID:", "Name:", "Head of Unit:","Head organization:", "Number of staff:", "Annual budget:","Expenses:" ,"Income:" ,"Loss:" ,"Description:" ,"Key Achievements:" };

				const std::string values[11] = {
					std::to_string(selNode->data->unitID),
					selNode->data->unitName,
					selNode->data->headOfUnit,
					(selNode->getParent() != nullptr ? selNode->getParent()->data->unitName : "0"),
					std::to_string(selNode->data->numStaff),
					std::to_string((int)selNode->data->annualBudget),
					std::to_string((int)selNode->data->expenses),
					std::to_string((int)selNode->data->income),
					std::to_string((int)selNode->data->loss),
					selNode->data->activityDesc,
					selNode->data->keyAchievements
				};

				static int selected = -1;
				for (int i = 0; i < 11; i++)
				{
					if (ImGui::Selectable(tags[i], selected == i, ImGuiSelectableFlags_SpanAllColumns))
					{
						selected = i;
					}
					ImGui::NextColumn();
					ImGui::Text(values[i].c_str()); ImGui::NextColumn();

				}
			}
			else
			{
				ImGui::Text("Select a Unit");
			}

			ImGui::End();
		}
		///Single unit data window end

		///Multi-unit data window TO DO --------------------------------------------------------------------------
		if (WINDOWS[Window::WINDOW_MULTI_UNIT_DATA])
		{
			ImGui::Begin("Multi Unit data", &WINDOWS[Window::WINDOW_MULTI_UNIT_DATA], ImGuiWindowFlags_ShowBorders);

			if (ImGui::Button("Update"))
			{
				update = true;
			}
			ImGui::BeginChild("sub1", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 60), false);
			ImGui::Columns(11, "multiunitdata");
			const char* headers[11] = { "Unit ID", "Name", "Head of Unit","Head organization", "Number of staff", "Annual budget","Expenses" ,"Income" ,"Loss" ,"Description" ,"Key Achievements" };
			ImGui::Separator();
			for (int i = 0; i < 11; i++)
			{
				ImGui::Text(headers[i]);
				ImGui::NextColumn();
			}
			ImGui::Separator();
			if (update)
			{
				if (!valueMatrix.empty())
				{
					valueMatrix.clear();
				}
				if (!selectedNodes.empty())
				{
					valueMatrix.resize(selectedNodes.size());

					for (auto &i : valueMatrix)
					{
						i.resize(11);
					}

					irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
					for (int i = 0; IT != selectedNodes.end(); IT++, i++)
					{
						std::string values[11] =
						{
							(std::to_string((*IT)->data->unitID)),
							(*IT)->data->unitName,
							(*IT)->data->headOfUnit,
							((*IT)->getParent() != nullptr ? (*IT)->getParent()->data->unitName : "0"),
							std::to_string((*IT)->data->numStaff),
							std::to_string((int)(*IT)->data->annualBudget) + "$",
							std::to_string((int)(*IT)->data->expenses) + "$",
							std::to_string((int)(*IT)->data->income) + "$",
							std::to_string((int)(*IT)->data->loss) + "$",
							(*IT)->data->activityDesc,
							(*IT)->data->keyAchievements,
						};

						for (int j = 0; j < 11; j++)
						{
							valueMatrix[i][j] = values[j];
						}
					}
				}
				update = false;
			}
			if (!valueMatrix.empty())
			{
				for (unsigned int i = 0; i < valueMatrix.size(); i++)
				{
					static int selected = -1;
					if (ImGui::Selectable(valueMatrix[i][0].c_str(), selected == i, ImGuiSelectableFlags_SpanAllColumns))
					{
						selected = i;
					}
					ImGui::NextColumn();
					for (int j = 1; j < 11; j++)
					{
						ImGui::Text(valueMatrix[i][j].c_str()); ImGui::NextColumn();
					}
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Spacing();

			}

			ImGui::EndChild();
			ImGui::End();
		}
		///Multi-unit data window end

		///Const data feed window
		{
			ImGui::SetNextWindowPos(ImVec2((float)screenSize.Width - 168.f, 22.f));
			ImGui::Begin("dataFeed", 0, ImVec2(168, 155), 0.6f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGui::Text("Summary data");
			ImGui::Columns(2, "datafeed", false);

			for (int i = 0; i < 6; i++)
			{
				ImGui::Text(summaryData[i][0].c_str());
			}
			ImGui::NextColumn();
			for (int i = 0; i < 6; i++)
			{
				ImGui::Text(summaryData[i][1].c_str());
			}

			if (!selectedNodes.empty())
			{
				if (summaryData[0][1] == "0")
				{
					int budget = 0, expenses = 0, income = 0, loss = 0, nstaff = 0;
					irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
					for (int i = 0; IT != selectedNodes.end(); IT++, i++)
					{
						budget += (int)(*IT)->data->annualBudget;
						expenses += (int)(*IT)->data->expenses;
						income += (int)(*IT)->data->income;
						loss += (int)(*IT)->data->loss;
						nstaff += (int)(*IT)->data->numStaff;
					}
					std::string values[6] =
					{
						std::to_string(selectedNodes.size()),
						std::to_string(nstaff),
						std::to_string(budget) + "$",
						std::to_string(expenses) + "$",
						std::to_string(income) + "$",
						std::to_string(loss) + "$"
					};

					for (int j = 0; j < 6; j++)
					{
						summaryData[j][1] = values[j];
					}

				}
			}
			else
			{
				for (int i = 0; i < 6; i++)
				{
					summaryData[i][1] = "0";
				}

			}
			ImGui::Columns(1);
			ImGui::Spacing();
			if (!selectedNodes.empty())
			{
				if (ImGui::Button("Details", ImVec2(148, 20)))
				{
					(WINDOWS[Window::WINDOW_MULTI_UNIT_DATA]) = true;
					update = true;
				}
			}
			ImGui::End();
		}
		///Const data feed window end

		///Search window
		if (WINDOWS[Window::WINDOW_SEARCH])
		{
			static const char* items[] = { "Name" };
			static int current = 0;
			ImGui::SetNextWindowPos(ImVec2(0.f, 22.f));
			ImGui::SetNextWindowSize(ImVec2(300.f, 120.f));
			ImGui::Begin("Search", &WINDOWS[Window::WINDOW_SEARCH], ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_ShowBorders);
			ImGui::Combo("Search by", &current, items, 4);


			static int selectedRec = -1;
			static const int maxRecs = 15;
			static int dispRecsH = 15;
			static int dispRecsW = 20;
			static int isFound = 0;
			static bool none = 0;
			switch (current)
			{
			case 0: {
				if (ImGui::InputText("Enter name", searchBuffer, 64, 0))
				{
					selectedRec = -1;
					none = 0;
				}
				if (searchBuff.size() > 0 && strcmp(searchBuffer, "") != 0 && selectedRec == -1 && none != 1)
				{
					using namespace std::regex_constants;
					std::regex e("\\b(" + std::string(searchBuffer) + ")([^ ]*)", ECMAScript | icase);
					std::smatch m;
					ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
					ImGui::SetNextWindowSize(ImVec2((float)(dispRecsW * 9),(float)(22 * dispRecsH)));
					ImGui::BeginTooltip();
					ImGui::PushAllowKeyboardFocus(false);
					int counter = 0;
					int longest = 0;
					int c = 0;
					for (unsigned int i = 0; i < searchBuff.size(); i++)
					{
						if (std::regex_search(searchBuff[i], m, e))
						{
							if (ImGui::Selectable(searchBuff[i].c_str(), selectedRec == i))
							{
								selectedRec = i;
								strcpy_s(searchBuffer, searchBuff[i].c_str());
							}
							counter++;
							if (searchBuff[i].length() > (unsigned int)longest)
							{
								longest = searchBuff[i].length();
							}
							none = 0;
						}
						if (counter >= maxRecs)
						{
							dispRecsH = 15;
						}
						else
						{
							dispRecsH = counter;
						}
						if (counter == 0)
						{
							none = 1;
						}
					}

					if (longest > 22)
					{
						dispRecsW = longest;
					}
					else
					{
						dispRecsW = 22;
					}

					ImGui::PopAllowKeyboardFocus();
					ImGui::EndTooltip();
				}
				if (ImGui::IsKeyPressed(13))
				{
					selectedRec = 0;
				}
				if (isFound == -1)
				{
					ImGui::Text("No records found");
				}
				if (ImGui::Button("Search", ImVec2(50, 20)))
				{
					bool found = false;
					Node* res = 0;
					for (int i = 0; i < tree.getNodeArraySize(); i++)
					{
						if (tree.getNodeArray()[i]->data->unitName == searchBuffer)
						{
							res = tree.getNodeArray()[i];
							selected = res->getISceneNode();
							cameraTarget = selected;
							anim = 1;
							DistVec = (selected->getPosition() - cameraTargetPosition);
							found = true;
						}
						if (!found)
						{
							isFound = -1;
						}
						else if (found)
						{
							isFound = 1;
						}
					}
				}
				break;
			};
			case 1: {
				break;
			};
			default:
				break;
			}

			ImGui::End();
		}
		///Search window end

		///Filter window
		if (WINDOWS[Window::WINDOW_FILTER])
		{
			ImGui::Begin("Filter", &WINDOWS[Window::WINDOW_FILTER], ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders);

			static const char* tabNames[] = { "Budget","Expenses", "Income","Loss","Staff" };
			static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
			//static const char* tabTooltips[numTabs] = { "Render Tab Tooltip","","","","Object Type Tooltip","","","","","Tired to add tooltips...","" };
			static int tabItemOrdering[numTabs] = { 0,1,2,3,4 };
			static int selectedTab = 0;
			static int optionalHoveredTab = 0;
			static int numVal = 0;

			TabLabels(numTabs, tabNames, selectedTab, 0, true, &optionalHoveredTab, &tabItemOrdering[0], true, true);
			//ImGui::Text("\nTab Page For Tab: \"%s\" here.\n", tabNames[selectedTab]);
			//if (optionalHoveredTab >= 0) ImGui::Text("Mouse is hovering Tab Label: \"%s\".\n\n", tabNames[optionalHoveredTab]);
			ImGui::Separator();

			ImGui::Text("Choose condition");
			ImGui::RadioButton(">", &option, 0); ImGui::SameLine();
			ImGui::RadioButton("<", &option, 1); ImGui::SameLine();
			ImGui::RadioButton("=", &option, 2);
			ImGui::InputText("Input value", filterBuffer, 64, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank);
			std::string inpVal(filterBuffer);
			if (!try_stoi(numVal, inpVal))
			{
				ImGui::Text("Please enter a valid value");
				selectedNodes.clear();
			}
			else if (try_stoi(numVal, inpVal))
			{
				switch (selectedTab)
				{
				case 0: {
					if (ImGui::Button("Filter", ImVec2(100.f, 20.f)))
					{
						selectedNodes.clear();
						switch (option)
						{
						case 0: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->annualBudget > std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 1: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->annualBudget < std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 2: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->annualBudget == std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						default: {
							break;
						}
						}
						if (!selectedNodes.empty())
						{
							isFiltered = true;
						}
						else
						{
							isFiltered = false;
						}

					}
					break;
				};
				case 1: {
					if (ImGui::Button("Filter", ImVec2(100.f, 20.f)))
					{
						selectedNodes.clear();
						switch (option)
						{
						case 0: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->expenses > std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 1: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->expenses < std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 2: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->expenses == std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						default: {
							break;
						}
						}
						if (!selectedNodes.empty())
						{
							isFiltered = true;
						}
						else
						{
							isFiltered = false;
						}

					}
					break;
				};
				case 2: {
					if (ImGui::Button("Filter", ImVec2(100.f, 20.f)))
					{
						selectedNodes.clear();
						switch (option)
						{
						case 0: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->income > std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 1: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->income < std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 2: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->income == std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						default: {
							break;
						}
						}
						if (!selectedNodes.empty())
						{
							isFiltered = true;
						}
						else
						{
							isFiltered = false;
						}

					}
					break;
				};
				case 3: {
					if (ImGui::Button("Filter", ImVec2(100.f, 20.f)))
					{
						selectedNodes.clear();
						switch (option)
						{
						case 0: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->loss > std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 1: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->loss < std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 2: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if (tree.getNodeArray()[i]->data->loss == std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						default: {
							break;
						}
						}
						if (!selectedNodes.empty())
						{
							isFiltered = true;
						}
						else
						{
							isFiltered = false;
						}

					}
					break;
				};
				case 4: {
					if (ImGui::Button("Filter", ImVec2(100.f, 20.f)))
					{
						selectedNodes.clear();
						switch (option)
						{
						case 0: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if ((int)tree.getNodeArray()[i]->data->numStaff > std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 1: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if ((int)tree.getNodeArray()[i]->data->numStaff < std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						case 2: {
							for (int i = 0; i < tree.getNodeArraySize(); i++)
							{
								if ((int)tree.getNodeArray()[i]->data->numStaff == std::stoi(filterBuffer))
								{
									selectedNodes.push_back(tree.getNodeArray()[i]);
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, selectedNodeTexture);
								}
								else
								{
									tree.getNodeArray()[i]->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
								}
							}
							break;
						};
						default: {
							break;
						}
						}
						if (!selectedNodes.empty())
						{
							isFiltered = true;
						}
						else
						{
							isFiltered = false;
						}

					}
					break;
				};
				default: {
					break;
				};
				}

				if (isFiltered && !selectedNodes.empty())
				{
					ImGui::Separator();
					if (ImGui::CollapsingHeader("Results"))
					{
						ImGui::BeginChild("filt1", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 185), true);
						irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
						for (; IT != selectedNodes.end(); IT++)
						{
							if (ImGui::TreeNode((*IT)->data->unitName.c_str()))
							{
								//ImGui::BeginChild("filt2");
								ImGui::Columns(2, "filterdata");

								const char* tags[11] = { "Unit ID:", "Name:", "Head of Unit:","Head organization:", "Number of staff:", "Annual budget:","Expenses:" ,"Income:" ,"Loss:" ,"Description:" ,"Key Achievements:" };

								const std::string values[11] = {
									std::to_string((*IT)->data->unitID),
									(*IT)->data->unitName,
									(*IT)->data->headOfUnit,
									((*IT)->getParent() != nullptr ? (*IT)->getParent()->data->unitName : "0"),
									std::to_string((*IT)->data->numStaff),
									std::to_string((int)(*IT)->data->annualBudget),
									std::to_string((int)(*IT)->data->expenses),
									std::to_string((int)(*IT)->data->income),
									std::to_string((int)(*IT)->data->loss),
									(*IT)->data->activityDesc,
									(*IT)->data->keyAchievements
								};

								static int selectedRec = -1;
								for (int i = 0; i < 11; i++)
								{
									if (ImGui::Selectable(tags[i], selectedRec == i, ImGuiSelectableFlags_SpanAllColumns))
									{
										selectedRec = i;
									}
									ImGui::NextColumn();
									ImGui::Text(values[i].c_str()); ImGui::NextColumn();

								}
								ImGui::Spacing();
								ImGui::Columns(1);
								if (ImGui::Button("GoTo", ImVec2(50, 20)))
								{
									selected = (*IT)->getISceneNode();
									cameraTarget = selected;
									anim = 1;
									DistVec = (selected->getPosition() - cameraTargetPosition);
								}
								//ImGui::EndChild();
								ImGui::TreePop();

							}
						}
						ImGui::EndChild();

					}
				}
				if (ImGui::Button("Drop", ImVec2(100.f, 20.f)))
				{
					irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
					for (; IT != selectedNodes.end(); IT++)
					{
						(*IT)->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
					}



					selectedNodes.clear();
					isFiltered = false;

				}
			}



			ImGui::End();
		}
		///Filter window end
		///Main Buttons;
		{
			{


				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.38f, 0.38f, 0.38f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.54f, 0.54f, 0.54f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.73f, 0.73f, 0.73f, 1.f));
				float bottomMargin = (float)(screenSize.Height - interfaceButton.Height) - 20;

				ImGui::SetNextWindowPos(ImVec2(screenSize.Width - interfaceButton.Width - spacing, bottomMargin));
				ImGui::SetNextWindowSize(ImVec2((float)screenSize.Width / 7.2f, (float)screenSize.Height / 12.6f));
				ImGui::Begin("WINDOWB1", 0, ImVec2(interfaceButton.Width, interfaceButton.Height), 0.f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
				if (ImGui::Button("Search", ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 10)))
				{
					searchBuff.clear();
					for (int i = 0; i < tree.getNodeArraySize(); i++)
					{
						searchBuff.push_back(tree.getNodeArray()[i]->data->unitName);
					}

					if (WINDOWS[Window::WINDOW_SEARCH] == 0)	WINDOWS[Window::WINDOW_SEARCH] = 1;
					else WINDOWS[Window::WINDOW_SEARCH] = 0;
				}
				ImGui::End();


				ImGui::SetNextWindowPos(ImVec2(screenSize.Width - interfaceButton.Width - spacing, (screenSize.Height - interfaceButton.Height * 2) - 20));
				ImGui::SetNextWindowSize(ImVec2((float)screenSize.Width / 7.2f, (float)screenSize.Height / 12.6f));
				ImGui::Begin("WINDOWB2", 0, ImVec2(interfaceButton.Width, interfaceButton.Height), 0.f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
				if (ImGui::Button("Filter", ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 10)))
				{
					if (WINDOWS[Window::WINDOW_FILTER] == 0) WINDOWS[Window::WINDOW_FILTER] = 1;
					else WINDOWS[Window::WINDOW_FILTER] = 0;
				}
				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(screenSize.Width - interfaceButton.Width - spacing, (screenSize.Height - interfaceButton.Height * 3) - 20));
				ImGui::SetNextWindowSize(ImVec2((float)screenSize.Width / 7.2f, (float)screenSize.Height / 12.6f));
				ImGui::Begin("WINDOWB3", 0, ImVec2(interfaceButton.Width, interfaceButton.Height), 0.f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
				if (ImGui::Button("Export Reports", ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 10)))
				{
					pdf::Document doc(pdf::create_file("report.pdf"));
					pdf::Font font = doc.font_load("standard; name=Helvetica; size=20");
					irr::core::list<Node*>::Iterator IT = selectedNodes.begin();
					pdf::Image logo = doc.image_load_file("logo.png");
					
					for (int i = 0; IT != selectedNodes.end(); IT++, i++)
					{
						
						doc.page_start(597.6, 848.68);
						doc.page().canvas().image(logo, 0, 0);
						


						doc.page().canvas().text_font(font);
						doc.page().canvas().move_to(10, 780);
						doc.page().canvas().line_to(590, 780);
						doc.page().canvas().path_paint("s");

						doc.page().canvas().text_start(50, 790);
						doc.page().canvas().text(((*IT)->data->unitName).c_str());
						doc.page().canvas().text_end();
						doc.page().canvas().text_start(50, 760);
						//doc.page().canvas().text(("ID: " + (std::to_string((*IT)->data->unitID))).c_str());
						//doc.page().canvas().text_translate_line(0, (-1)*font.height());
						//doc.page().canvas().text(("Unit Name: " + (*IT)->data->unitName).c_str());
						//doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Director: " + (*IT)->data->headOfUnit).c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Head Organization: " + ((*IT)->getParent() != nullptr ? (*IT)->getParent()->data->unitName : "0")).c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Number of staff: " + (std::to_string((*IT)->data->numStaff)) + " members").c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Annual Budget: " + (std::to_string((int)(*IT)->data->annualBudget)) + "$").c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Expenses: " + (std::to_string((int)(*IT)->data->expenses)) + "$").c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Income: " + (std::to_string((int)(*IT)->data->income)) + "$").c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Loss: " + (std::to_string((int)(*IT)->data->loss)) + "$").c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Description: " + (*IT)->data->activityDesc).c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text(("Key Achievements: " + (*IT)->data->keyAchievements).c_str());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text_translate_line(0, (-1)*font.height());
						doc.page().canvas().text_end();
						doc.page_end();
					}
					doc.finalize();
				}
				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(screenSize.Width - interfaceButton.Width - spacing, (screenSize.Height - interfaceButton.Height * 4) - 20));
				ImGui::SetNextWindowSize(ImVec2((float)screenSize.Width / 7.2f, (float)screenSize.Height / 12.6f));
				ImGui::Begin("WINDOWB4", 0, ImVec2(interfaceButton.Width, interfaceButton.Height), 0.f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
				if (ImGui::Button("Return camera", ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 10)))
				{
					r = 2000.f;
					theta = 1.8f;
					phi = -1.15f;
					selected = tree.getRoot()->getISceneNode();
					cameraPosition = core::vector3df(0, 0, -500);
					cameraTargetPosition = core::vector3df(0, 0, 0);
					camera->setPosition(cameraPosition);
					camera->setTarget(cameraTargetPosition);
				}
				ImGui::End();

				ImGui::PopStyleColor(3);
			}
		}
		///Main Buttons end;


		///Log window
		if (WINDOWS[Window::WINDOW_LOG])
		{
			if (!WINDOWS_COLLAPSED[Window::WINDOW_LOG])
			{
				ImGui::SetNextWindowPos(ImVec2(0.f, (float)driver->getScreenSize().Height - 100));
				ImGui::SetNextWindowSize(ImVec2((float)driver->getScreenSize().Width, 100));
			}
			else
			{
				ImGui::SetNextWindowPos(ImVec2(0.f, (float)driver->getScreenSize().Height - 20));
				ImGui::SetNextWindowSize(ImVec2(70, 100));
			}
			log.Draw("Log", WINDOWS_COLLAPSED[Window::WINDOW_LOG], &WINDOWS[Window::WINDOW_LOG], (ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove));
		}
		///Log window end

		if (WINDOWS[Window::WINDOW_TOOLS])
		{
			ImGui::Begin("ToolBox", &WINDOWS[Window::WINDOW_TOOLS], ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
			ImGui::SetWindowSize(ImVec2(150, 300));
			if (VIEWMODES[UserMode::USERMODE_SPECTATOR])
			{
				if (ImGui::MenuItem(ICON_FA_HAND_POINTER" Selector", NULL, TOOLS_S[ToolsSpectator::TS_SELECTOR]))
				{
					if (ImGui::IsMouseClicked(0)) {
						if (TOOLS_S[ToolsSpectator::TS_SELECTOR] == 0)
						{
							toolSelectSpectator(ToolsSpectator::TS_SELECTOR);
						}
						else
						{
							TOOLS_S[ToolsSpectator::TS_SELECTOR] = 0;
						}
					}
				}

			}
			else if (VIEWMODES[UserMode::USERMODE_EDITOR])
			{
				if (ImGui::Button("Center"))
				{
					cameraTarget = center;
				}
				if (ImGui::MenuItem(ICON_FA_HAND_POINTER" Selector", NULL, TOOLS_E[ToolEditor::TE_SELECTOR]))
				{
					if (ImGui::IsMouseClicked(0)) {
						if (TOOLS_E[ToolEditor::TE_SELECTOR] == 0)
						{
							toolSelectEditor(ToolEditor::TE_SELECTOR);
						}
						else
						{
							TOOLS_E[ToolEditor::TE_SELECTOR] = 0;
						}
					}
				}
				if (ImGui::MenuItem(ICON_FA_PLUS_CIRCLE" ADD NODE", NULL, TOOLS_E[ToolEditor::TE_ADD_NODE]))
				{
					if (ImGui::IsMouseClicked(0)) {
						if (TOOLS_E[ToolEditor::TE_ADD_NODE] == 0)
						{
							toolSelectEditor(ToolEditor::TE_ADD_NODE);

						}
						else
						{
							TOOLS_E[ToolEditor::TE_ADD_NODE] = 0;
						}
					}
				}
				if (ImGui::MenuItem(ICON_FA_MINUS_CIRCLE" DELETE NODE", NULL, TOOLS_E[ToolEditor::TE_DELETE_NODE]))
				{
					if (ImGui::IsMouseClicked(0))
					{
						if (TOOLS_E[ToolEditor::TE_DELETE_NODE] == 0)
						{
							toolSelectEditor(ToolEditor::TE_DELETE_NODE);
						}
						else
						{
							TOOLS_E[ToolEditor::TE_DELETE_NODE] = 0;
						}
					}
				}
				if (ImGui::MenuItem(ICON_FA_LINK" ADD LINK", NULL, TOOLS_E[ToolEditor::TE_ADD_LINK]))
				{
					if (ImGui::IsMouseClicked(0))
					{
						if (TOOLS_E[ToolEditor::TE_ADD_LINK] == 0)
						{
							toolSelectEditor(ToolEditor::TE_ADD_LINK);
							isSecond = false;
						}
						else
						{
							TOOLS_E[ToolEditor::TE_ADD_LINK] = 0;
						}
					}
				}
				if (ImGui::MenuItem(ICON_FA_UNLINK" DELETE LINK", NULL, TOOLS_E[ToolEditor::TE_DELETE_LINK]))
				{
					if (ImGui::IsMouseClicked(0))
					{
						if (TOOLS_E[ToolEditor::TE_DELETE_LINK] == 0)
						{
							toolSelectEditor(ToolEditor::TE_DELETE_LINK);
							isSecond = false;
						}
						else
						{
							TOOLS_E[ToolEditor::TE_DELETE_LINK] = 0;
						}
					}
				}
			}
			ImGui::End();
		}
		/////////////////////////////////////////////////////////////////////////
		///ImGui::ShowTestWindow();
		///Interface end
		///Functionality 
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			///Select Box
			if (ImGui::IsMouseDown(0) && !firstCornerSet)
			{
				isFiltered = false;
				firstCorner.X = (int)ImGui::GetMousePos().x;
				firstCorner.Y = (int)ImGui::GetMousePos().y;
				firstCornerSet = true;
			}
			if (ImGui::IsMouseDragging(0) && ImGui::IsMouseDown(0))
			{
				secondCorner.X = (int)ImGui::GetMousePos().x;
				secondCorner.Y = (int)ImGui::GetMousePos().y;

				if (secondCorner.X < firstCorner.X && secondCorner.Y < firstCorner.Y)
				{
					selectBox.UpperLeftCorner = secondCorner;
					selectBox.LowerRightCorner = firstCorner;
				}
				else if (secondCorner.X < firstCorner.X)
				{
					int differenceX = firstCorner.X - secondCorner.X;
					selectBox.UpperLeftCorner = firstCorner;
					selectBox.LowerRightCorner = secondCorner;
					selectBox.UpperLeftCorner.X -= differenceX;
					selectBox.LowerRightCorner.X += differenceX;
				}
				else if (secondCorner.Y < firstCorner.Y)
				{
					int differenceY = firstCorner.Y - secondCorner.Y;
					selectBox.UpperLeftCorner = firstCorner;
					selectBox.LowerRightCorner = secondCorner;

					selectBox.UpperLeftCorner.Y -= differenceY;
					selectBox.LowerRightCorner.Y += differenceY;
				}
				else
				{
					selectBox.UpperLeftCorner = firstCorner;
					selectBox.LowerRightCorner = secondCorner;
				}
				driver->draw2DRectangle(SColor(20, 22, 144, 255), selectBox);
				driver->draw2DRectangleOutline(selectBox, SColor(255, 22, 144, 255));
			}
			///Select box end



			Functionality::highlightHoveredNode(selectedNodes, hovered, lastHovered, selectedNodeTexture, defaultNodeTexture, highlightSelectedNodeTexture, highlightDefaultNodeTexture, isNodeHovered, device, CollMan);



			///Zoom control
			if (distance < 14000)
			{
				if (distance >= 14000)
				{
					r = 13999.f;
				}
				if (ImGui::GetIO().MouseWheel < 0.0f)
				{
					if (CAMERAMODES[CameraMode::CAMERA_FREE])
						cameraPosition += ((cameraPosition - cameraTargetPosition) / distance) * (25 * (-1) * ImGui::GetIO().MouseWheel);
					else if (CAMERAMODES[CameraMode::CAMERA_ORBIT])
						r -= 30 * ImGui::GetIO().MouseWheel;
				}
			}
			if (distance > 100)
			{
				if (distance <= 100)
				{
					r = 101.f;
				}
				if (ImGui::GetIO().MouseWheel > 0.0f)
				{
					if (CAMERAMODES[CameraMode::CAMERA_FREE])
						cameraPosition -= ((cameraPosition - cameraTargetPosition) / distance) * (25 * ImGui::GetIO().MouseWheel);
					if (CAMERAMODES[CameraMode::CAMERA_ORBIT])
						r -= 30 * ImGui::GetIO().MouseWheel;
				}
			}
			///Zoom control end

			diffX = (float)((cursorPosition.X - (s32)ImGui::GetMousePos().x));
			diffY = (float)((cursorPosition.Y - (s32)ImGui::GetMousePos().y));

			///Right click camera rotation
			///Right click camera rotation with quaternion-based to avoid gimbal lock
			if (ImGui::IsMouseDown(1) && IsMouseMoved)
			{
				float sensitivity = 0.002f;

				if (CAMERAMODES[CameraMode::CAMERA_ORBIT])
				{
					// Орбитальная камера с ограничением углов
					phi -= diffX * sensitivity;
					theta = std::clamp(theta - diffY * sensitivity, 0.1f, 3.04f);
				}
				else if (CAMERAMODES[CameraMode::CAMERA_FREE])
				{
					// FPS камера с кватернионами для избежания блокировки
					static float yaw = 0.0f;
					static float pitch = 0.0f;

					yaw += diffX * sensitivity;
					pitch = std::clamp(pitch + diffY * sensitivity, -1.5f, 1.5f); // Ограничиваем ±85°

					// Расчет направления через углы Эйлера
					core::vector3df front;
					front.X = cos(pitch) * sin(yaw);
					front.Y = sin(pitch);
					front.Z = cos(pitch) * cos(yaw);
					front.normalize();

					// Обновляем позицию цели
					cameraTargetPosition = cameraPosition + front * 100.0f;

					// Сохраняем углы для совместимости с остальным кодом
					theta = acos(front.Y); // pitch angle
					phi = atan2(front.X, front.Z); // yaw angle
				}
			}

			

			///WASD movement with RMB (camera-relative movement)
			if (ImGui::IsMouseDown(1))
			{
				float moveSpeed = 500.0f * frameDeltaTime;

				// Альтернативный расчет векторов - более надежный
				core::vector3df forward = camera->getTarget() - camera->getPosition();
				forward.Y = 0; // Игнорируем вертикальную компоненту для горизонтального движения
				forward.normalize();

				core::vector3df right = forward.crossProduct(core::vector3df(0, 1, 0));
				right.normalize();

				// Движение в направлениях относительно камеры
				if (EventReceiver.mKeyW) {
					cameraPosition += forward * moveSpeed;
					cameraTargetPosition += forward * moveSpeed;
				}
				if (EventReceiver.mKeyS) {
					cameraPosition -= forward * moveSpeed;
					cameraTargetPosition -= forward * moveSpeed;
				}
				if (EventReceiver.mKeyA) {
					cameraPosition += right * moveSpeed;
					cameraTargetPosition += right * moveSpeed;
				}
				if (EventReceiver.mKeyD) {
					cameraPosition -= right * moveSpeed;
					cameraTargetPosition -= right * moveSpeed;
				}

				// Вертикальное движение
				if (EventReceiver.mKeyQ) {
					cameraPosition.Y -= moveSpeed;
					cameraTargetPosition.Y -= moveSpeed;
				}
				if (EventReceiver.mKeyE) {
					cameraPosition.Y += moveSpeed;
					cameraTargetPosition.Y += moveSpeed;
				}
			}

			///CTRL+LEFT click selection
			if (ImGui::IsMouseClicked(0))
			{
				ISceneNode *selectedNode = CollMan->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(), 0, true);
				irr::core::list<irr::scene::ISceneNode *> children = smgr->getRootSceneNode()->getChildren();
				core::list< scene::ISceneNode* >::Iterator IT = children.begin();
				for (; IT != children.end(); ++IT)
				{
					ISceneNode *node = (*IT);

					if (node == camera || node == grid || node == center) continue;

					//Ignore camera and terrain node
					if (node == selectedNode)
					{
						Node* nNode = tree.findNode(*IT);
						if (!Functionality::isFound(selectedNodes, node))
						{
							selectedNodes.push_back(nNode);
							node->setMaterialTexture(0, selectedNodeTexture);
						}
					}
				}
			}
			///CTRL+LEFT click selection end
			/*
			if (ImGui::IsMouseClicked(1) && EventReceiver.mCtrlPressed)
			{
			ISceneNode *selectedNode = CollMan->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(), 0, true);
			irr::core::list<irr::scene::ISceneNode *> children = smgr->getRootSceneNode()->getChildren();
			core::list< scene::ISceneNode* >::Iterator IT = children.begin();
			for (; IT != children.end(); ++IT)
			{
			ISceneNode *node = (*IT);

			if (node == camera || node == grid || node == center) continue;

			//Ignore camera and terrain node
			if (node == selectedNode)
			{
			irr::core::list<Node* nNode = tree.findNode(*IT);
			if (Functionality::isFound(selectedNodes, node))
			{
			selectedNodes.erase(IT);
			node->setMaterialTexture(0, selectedNodeTexture);
			}
			}
			}
			}
			*/

			if (ImGui::IsMouseClicked(0) && !EventReceiver.mCtrlPressed)
			{
				core::list<Node*>::Iterator IT = selectedNodes.begin();
				for (; IT != selectedNodes.end(); IT++)
				{
					(*IT)->getISceneNode()->setMaterialTexture(0, defaultNodeTexture);
				}
				selectedNodes.clear();
				isFiltered = false;

				selected = CollMan->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(), 0, true);
				if (TOOLS_E[ToolEditor::TE_ADD_NODE]) {
					if (selected) {
						selNode = tree.findNode(selected);

						if (selNode)
						{

						}
					}
				}
				else
					if (TOOLS_E[ToolEditor::TE_DELETE_NODE])
					{
						if (selected) {
							selNode = tree.findNode(selected);

							if (selNode)
							{
								if (selNode->getParent() != nullptr) {
									selNode->removeParent();
								}
								for (int i = 0; i < selNode->getnChildNodes(); i++) {
									(selNode->getChildNodes())[i]->removeParent();
								}
							}
							selected->remove();
							//TO DO: Delete from LogicalTree + figs array
						}
					}
					else
						if (TOOLS_E[ToolEditor::TE_ADD_LINK])
						{
							if (isSecond) {
								if (selNode) {
									if (selected) {
										if (selNode->getParent() == nullptr) {
											if ((tree.getDepth((tree.findNode(selected)))) > (tree.getDepth(selNode))) {
												selNode->setParent(*(tree.findNode(selected)));
											}
										}
										else
											if ((tree.findNode(selected))->getParent() == nullptr) {
												if ((tree.getDepth((tree.findNode(selected)))) < (tree.getDepth(selNode))) {
													(tree.findNode(selected))->setParent(*selNode);
												}
											}
									}
								}
							}
							if (selected) {
								selNode = tree.findNode(selected);
								isSecond = true;
							}
							else {
								isSecond = false;
							}
						}
						else
							if (TOOLS_E[ToolEditor::TE_DELETE_LINK])
							{
								if (isSecond) {
									if (selNode->getParent() == tree.findNode(selected)) {
										selNode->removeParent();
									}
									else
										if (selNode == (tree.findNode(selected))->getParent()) {
											(tree.findNode(selected))->removeParent();
										}
								}
								if (selected) {
									selNode = tree.findNode(selected);
									isSecond = true;
								}
								else {
									isSecond = false;
								}
							}
			}

			///Unit data selector
			if (TOOLS_S[ToolsSpectator::TS_SELECTOR]) {
				if (ImGui::IsMouseClicked(0) && !EventReceiver.mCtrlPressed) {
					selected = CollMan->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(), 0, true);
					if (selected)
					{
						if (selected != camera)
						{
							anim = 1;
							DistVec = (selected->getPosition() - cameraTargetPosition);
							selNode = tree.findNode(selected);
							if (selNode)
							{
								if (WINDOWS[WINDOW_UNIT_DATA] == false) WINDOWS[WINDOW_UNIT_DATA] = true;
							}
						}

					}
				}
			}

		}
		///Movement animation
		if (selected && anim && CAMERAMODES[CameraMode::CAMERA_ORBIT]) {
			if (cameraTargetPosition.getDistanceFrom(selected->getPosition()) < 4)
			{
				anim = 0;
			}
			if (anim == 1)
			{
				cameraTargetPosition += ((DistVec / 20));
				if (r < 500)
				{
					r += 50;
					if ((500 - r) < 70)
					{
						r = 500;
					}
				}
				else if (r > 500)
				{
					r -= 50;
					if ((r - 500) < 70)
					{
						r = 500;
					}
				}
			}
			else
			{

				cameraTarget = selected;
				cameraTargetPosition = cameraTarget->getAbsolutePosition();
			}
		}
		///Movement animation  end
		if (selected)
		{
			if (!lastSelected)
			{
				lastSelected = selected;
			}
			if (lastSelected != selected && !Functionality::isFound(selectedNodes, lastSelected) && !Functionality::isFound(filteredNodes, lastSelected))
			{
				lastSelected->setMaterialTexture(0, defaultNodeTexture);
			}
			selected->setMaterialTexture(0, selectedNodeTexture);
			lastSelected = selected;


		}
		if (ImGui::IsMouseReleased(0))
		{
			irr::core::list<irr::scene::ISceneNode *> children = smgr->getRootSceneNode()->getChildren();
			core::list< scene::ISceneNode* >::Iterator IT = children.begin();
			for (; IT != children.end(); ++IT)
			{
				ISceneNode *node = (*IT);

				if (node == camera || node == grid || node == center) continue;

				//Ignore camera and terrain node
				vector3df vec = (*IT)->getPosition();
				vector2d<irr::s32> screenpos = CollMan->getScreenCoordinatesFrom3DPosition(vec);
				vector2di v(screenpos.X, screenpos.Y);
				if (selectBox.isPointInside(v))
				{
					Node* nNode = tree.findNode(*IT);
					if (!Functionality::isFound(selectedNodes, node))
					{
						selectedNodes.push_back(nNode);
						node->setMaterialTexture(0, selectedNodeTexture);
					}
				}
			}
			firstCornerSet = false;
			selectBox.UpperLeftCorner = selectBox.LowerRightCorner = firstCorner = secondCorner = irr::core::vector2di(0, 0);
		}
		///	Camera rotation with mouse
		if (CAMERAMODES[CameraMode::CAMERA_ORBIT])
		{
			cameraPosition.X = (r * sin(theta) * cos(phi)) + cameraTargetPosition.X;
			cameraPosition.Y = (r * cos(theta)) + cameraTargetPosition.Y;
			cameraPosition.Z = (r * sin(theta) * sin(phi)) + cameraTargetPosition.Z;
		}
		else if (CAMERAMODES[CameraMode::CAMERA_FREE])
		{
			cameraTargetPosition.X = cameraPosition.X + (100 * sin(theta) * cos(phi));
			cameraTargetPosition.Y = cameraPosition.Y + (100 * cos(theta));
			cameraTargetPosition.Z = cameraPosition.Z + (100 * sin(theta) * sin(phi));
		}

		cursorPosition.X = (s32)ImGui::GetMousePos().x;
		cursorPosition.Y = (s32)ImGui::GetMousePos().y;

		camera->setTarget(cameraTargetPosition);
		camera->setPosition(cameraPosition);

		///	Camera rotation with mouse

		///Functionality end

		///Mandatory function executions
		ImGui::PopFont();
		pGUI->drawAll();

		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTargetPosition);
		driver->endScene();

		///Mandatory function executions end

		///FPS COUNTER
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw tmp(L"Bastau Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
		///FPS COUNTER END
	}
	///MAIN LOOP END
	pGUI->resetFonts();
	pGUI->drop();
	device->drop();

	std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();

	return 0;
}

///Function body declaration
void toolSelectEditor(ToolEditor tool)
{
	if (TOOLS_E[tool] == true)	return;
	for (int i = 0; i < MAX_EDITOR_TOOLS; i++)
	{
		TOOLS_E[i] = false;
	}
	TOOLS_E[tool] = true;
}
void toolSelectSpectator(ToolsSpectator tool)
{
	if (TOOLS_S[tool] == true)	return;
	for (int i = 0; i < MAX_SPECTATOR_TOOLS; i++)
	{
		TOOLS_S[i] = false;
	}
	TOOLS_S[tool] = true;
}
void switchViewMode(UserMode mode)
{
	if (VIEWMODES[mode] == true)
	{
		return;
	}
	else if (mode == USERMODE_EDITOR)
	{
		VIEWMODES[USERMODE_SPECTATOR] = false;
		toolSelectEditor(TE_SELECTOR);
	}
	else
	{
		VIEWMODES[USERMODE_EDITOR] = false;
		toolSelectSpectator(TS_SELECTOR);
	}
	VIEWMODES[mode] = true;
}
bool try_stoi(int & i, const std::string & s)
{
	try {
		size_t pos;
		i = std::stoi(s, &pos);
		return pos == s.size();
	}
	catch (const std::invalid_argument&) {
		return false;
	}
}


