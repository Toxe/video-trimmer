#pragma once

class EventHandler;
class UI;
class MainWindow;

void register_events(EventHandler& event_handler, MainWindow& window, UI& ui);
