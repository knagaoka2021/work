using System;

public class UndoRedoManager {

    private UndoRedo undoRedo = new UndoRedo (int.MaxValue);

    public void Undo () {
        undoRedo.Undo ();
    }
    public void Redo () {
        undoRedo.Redo ();
    }
    public void Regist(ICommand command){
        undoRedo.Regist(command);
    }
    public void Clear () {
        undoRedo.Clear ();
    }

}