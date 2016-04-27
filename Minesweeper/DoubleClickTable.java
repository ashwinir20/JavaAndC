package com.game.mines.client;


import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.Element;
import com.google.gwt.event.dom.client.MouseEvent;
import com.google.gwt.event.shared.EventHandler;
import com.google.gwt.user.client.Event;
import com.google.gwt.dom.client.TableRowElement;
import com.google.gwt.dom.client.TableCellElement;
import com.google.gwt.event.dom.client.ContextMenuEvent;





public class DoubleClickTable extends FlexTable {

	
	class MyCell extends Cell {
        protected MyCell(int rowIndex, int cellIndex) {
            super(rowIndex, cellIndex);
        }
    }

    public Cell getCellForEvent(ContextMenuEvent event) {
        Element td = getEventTargetCell(Event.as(event.getNativeEvent()));
        if (td == null) {
          return null;
        }

        int row = TableRowElement.as(td.getParentElement()).getSectionRowIndex();
        int column = TableCellElement.as(td).getCellIndex();
        return new MyCell(row, column);
    }
}
