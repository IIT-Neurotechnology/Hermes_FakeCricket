import customtkinter
from Livexperiment_option1 import AppOption1
from Videoanalysis_option2 import AppOption2
from Testcontrol_option3 import AppOption3

customtkinter.set_appearance_mode("Dark")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"

class PreAppDialog(customtkinter.CTkToplevel):
    def __init__(self, parent):
        super().__init__(parent)
        self.title("Robocricket GUI")
        self.geometry("350x250")
        self.grab_set()  # Make this window modal
        self.option = None

        self.label = customtkinter.CTkLabel(self, text="Choose an option to start the app:", font=customtkinter.CTkFont(size=20))
        self.label.pack(pady=20)

        self.option1_button = customtkinter.CTkButton(self, text="Live Experiment", command=lambda: self.on_button_click("Live Experiment"))
        self.option1_button.pack(pady=10)

        self.option2_button = customtkinter.CTkButton(self, text="Video Analysis", command=lambda: self.on_button_click("Video Analysis"))
        self.option2_button.pack(pady=10)
        
        self.option3_button = customtkinter.CTkButton(self, text="Test Control", command=lambda: self.on_button_click("Test Control"))
        self.option3_button.pack(pady=10)

    def on_button_click(self, option):
        self.option = option
        print(f"{option} selected")
        self.destroy()  # Close the dialog

if __name__ == "__main__":
    root = customtkinter.CTk()
    root.withdraw()
    dialog = PreAppDialog(root)
    root.wait_window(dialog)

    if dialog.option == "Live Experiment":
        app = AppOption1()
    elif dialog.option == "Video Analysis":
        app = AppOption2()
    elif dialog.option == "Test Control":
        app = AppOption3()
    app.mainloop()
