using System;
using System.Collections.Generic;
using System.Linq;

public class UndoRedo {
    public int maxRecordNum;

    LinkedList<ICommand> undoCommandList = new LinkedList<ICommand> ();
    LinkedList<ICommand> redoCommandList = new LinkedList<ICommand> ();

    public UndoRedo (int maxRecordNum) {
        this.maxRecordNum = maxRecordNum;
    }

    public void Undo () {
        if (undoCommandList.Count > 0) {
            ICommand c = undoCommandList.First.Value;
            undoCommandList.RemoveFirst ();

            // 元に戻す処理実施
            c.UnExecute ();

            // 最大保有数制御
            if (redoCommandList.Count + 1 > maxRecordNum) {
                redoCommandList.RemoveLast ();
            }
            // やり直し管理に確保
            if (redoCommandList.Count + 1 <= maxRecordNum) {
                redoCommandList.AddFirst (c);
            }
        }
    }
    public void Redo () {
        if (redoCommandList.Count > 0) {
            ICommand c = redoCommandList.First.Value;
            redoCommandList.RemoveFirst ();

            // 元に戻す処理実施
            c.Execute ();

            // 最大保有数制御
            if (undoCommandList.Count + 1 > maxRecordNum) {
                undoCommandList.RemoveLast ();
            }
            // やり直し管理に確保
            if (undoCommandList.Count + 1 <= maxRecordNum) {
                undoCommandList.AddFirst (c);
            }
        }
    }
    public void Regist (ICommand command) {
        // 最大保有数制御
        if (undoCommandList.Count + 1 > maxRecordNum) {
            undoCommandList.RemoveLast ();
        }
        // やり直し管理に確保
        if (undoCommandList.Count + 1 <= maxRecordNum) {
            undoCommandList.AddFirst (command);
        }
        // 元に戻す格納によるやり直し初期化
        redoCommandList.Clear();
    }
    public void Clear(){
        undoCommandList.Clear();
        redoCommandList.Clear();
    }

}