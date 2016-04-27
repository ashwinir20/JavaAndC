 package com.game.mines.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.NativeEvent;
import com.google.gwt.dom.client.TableCellElement;
import com.google.gwt.dom.client.TableRowElement;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.FlexTable.FlexCellFormatter;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.event.dom.client.ContextMenuHandler;
import com.google.gwt.event.dom.client.ContextMenuEvent;
import com.google.gwt.event.dom.client.MouseEvent;
import com.google.gwt.event.shared.EventHandler;
import java.util.Random;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.widget.client.*;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.client.ui.Label;




/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Mines extends  DoubleClickTable   implements EntryPoint {
	/**
	 * The message displayed to the user when the server cannot be reached or
	 * returns an error.
	 */
	private static final String SERVER_ERROR = "An error occurred while "
			+ "attempting to contact the server. Please check your network "
			+ "connection and try again.";


	
	
	
		int counter = 0 ;
		int maxMines = 1;

	
	 	final int gridHeight = 10;
	 	
	 	final int gridWidth = 10;
	
	 	int count ;
	 
	 	int rightClickCounter = 0;
	 	
	 	
		private int x;
		
		private int y;
		
		
	
		public int getX(){
			
			return x;
		}
		
		public void setX(int x){
			this.x = x;
		}
		
		
		public int getY(){
			return y;
		}
		public void setY(int y){
			this.y = y;
			
		}
		
		public void setCoordinates(int x , int y){
			this.x = x;
			this.y =y;
			
		}
	 
	 DoubleClickTable flex = new DoubleClickTable();
	 
	 Mines[] points = new Mines[20];
	 
	 
	 Button button = new Button();
	
	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad() {
		
		
		
		
	 	//setting all 100 cells to N
		//not a mine 
	 	for(int i = 0; i < gridWidth ;i++){
		
  			for(int j = 0; j < gridHeight ; j++){
  			
  				flex.setWidget(i, j, new Button("N"));
  				flex.getElement().getStyle().setProperty("backgroundColor", "grey");
  			}
		}
		
	 //setting mines - 15
	 
	 	
	 		int k = 0;
	 		while(maxMines < 16){
			int  i = randInt(0,10);
			
			int j = randInt(0,10);
			
			Widget widget = flex.getWidget(i, j);
			
			if(!(((Button) widget).getText()).equals("M")){
				flex.setWidget(i, j, new Button("M"));
				flex.getElement().getStyle().setProperty("backgroundColor", "grey");
				
				points[k] = new Mines();
				
 				points[k].setCoordinates(i, j);
 				System.out.println("k is:" + k + "row" + i + "col" + j);
				maxMines = maxMines + 1;
				k = k+1;
			}
	 		}
	 			
	 	
	 		
	 		
	 		
	 flex.addClickHandler( new ClickHandler(){
		 
		 
			
			public void onClick(ClickEvent event) {
				
				int column = flex.getCellForEvent(event).getCellIndex();
	            int row = flex.getCellForEvent(event).getRowIndex();
	            System.out.println("the cell index : " + column);
	            System.out.println("the row index is :" + row);
	            Button src = (Button)flex.getWidget(row, column);
	            
	            //if you click on button that is not a mine 
	            if(src.getText().equals("N")){
	            	//Window.alert("this isn't a mine");
	            	System.out.println("what is src?"+src.getClass());
	            	
	            	cascade(row,column);
	            	
	            	
	            	
	            }
	            else 
	            	if(src.getText().equals("M")){
	            		Window.alert("this is a mine!!");
	            		
	            		for(int i = 0 ; i < points.length ; i++){
	            			
	            			System.out.println("length:" + points.length);
	            			int r = points[i].getX();
	            			int c = points[i].getY();
	            			flex.setWidget(r, c, new Image("/images/mine.png"));
	            		}
	            	}
				
			}
		 }
		 
		 ); 
 
	 flex.sinkEvents(Event.ONCONTEXTMENU);
	 
	 

	 flex.addHandler( new ContextMenuHandler(){
	 		
	 		@Override
	 		public void onContextMenu(ContextMenuEvent event){
	 			
	 			event.preventDefault();
					
	 			Cell cell = flex.getCellForEvent(event);
	 	        
	 			int row = cell.getRowIndex();
	 			
	 			int column = cell.getCellIndex();
	 			
	 			
	 			
	 			flex.setWidget(row, column, new Label("F"));
	 	        rightClickCounter++;
	 			
	 			if(rightClickCounter == 15){
	 				Window.alert("checking if you have won!");
	 				CheckWin();
	 			}
	 				
	 		}
	 	}, ContextMenuEvent.getType());
	 	
	
	
	 RootPanel.get("container").add(flex);

		
	}
	

	public void CheckWin(){
		
		for( int i = 0 ; i < gridWidth ; i++ ){
			for (int j = 0 ; j < gridHeight ; j++){
				
  	          Widget w = flex.getWidget(i, j);
  	          
  	        if(w instanceof Button && ((Button) w).getText().equals("M")){
	        	
	        		  Window.alert("You lose! You got the mines wrong!");
	        		  
	        		  for( int l = 0 ; l < points.length ; l ++){
	        			  
	        			 
	        			 int ro = points[l].getX();
	  	        			
	  	        		int co = points[l].getY();
	  	        		
	  	        		System.out.println("row is:" +ro +"column is:"+co);
	  	        		flex.setWidget(ro, co, new Image("/images/mine.png"));
	  	        }
	        		  
  	        }
			}
		}
	       
		Window.alert("You got all the Mines!");
		
	        			
	
		
}
		
	
	
public void cascade(int i , int j){
		
	if( (i >= 0) && (i < gridWidth) && (j >= 0) && (j < gridHeight) ){
		
		
		count = getMines(i,j);
		
		
		if(count == 0 && isMine(i,j) == false && isOpen(i,j) == true){
			
			
			flex.setWidget(i, j,  new Label(""));
			
			cascade(i, j-1);//left
    		cascade(i, j+1);//right
	    	cascade(i+1, j);//down
    		cascade(i-1, j);//up
			cascade(i-1,j-1);//upper left
	    	cascade(i-1,j+1);//upper right
    		cascade(i+1,j-1);//lower left
			cascade(i+1,j+1);//lower right
			
		}
		else if(count != 0) {
			flex.setWidget(i, j, new Label(count + ""));
			
		}
	}
	
	else
		return;
	
	
	
		
				
}
	
	
	public boolean checkBounds(int p , int q){
		
		
        if( (p >= 0) && (p < gridWidth) && (q >= 0) && ( q < gridHeight) ){
        	
        	return true;
        }
        else
        	return false;
		
	}
		
	
	
	
	public boolean isMine(int a, int b){
		
		Widget w = flex.getWidget(a, b);
		
		if( w instanceof Button && ((Button) w).getText().equals("M")){
			return true;
		}
		else
		
		return false;
		
		
			
		
	}
	
	public boolean isOpen(int c, int d){
		
		
		Widget w = flex.getWidget(c, d);
		
		if(w instanceof Button && ((Button) w).getText().equals("N")){
			
			return true;
		
		}
		else
			return false;
	}
	
	public int getMines(int x, int y){
	
		count =0 ;
		if(( (x >=0) && (x < gridWidth)) && ( ((y-1) >=0) && ((y-1 ) < gridHeight ))){
		 	Widget w1 = flex.getWidget(x , y-1 );
		 	
		 	if( (w1 instanceof Button) && (w1.isVisible()) ){
				if( ((Button) w1).getText().equals("M")){
					count++;
				}
			}
		 	
			}
			
			//x,y+1
			 if( ( (x >=0) && (x < gridWidth)) && ( ((y+1) >=0) && ((y+1 ) < gridHeight ))){
			
			Widget w2 = flex.getWidget(x , y+1);
			
			if((w2 instanceof Button) && (w2.isVisible())){	
				if( ((Button) w2).getText().equals("M")){
					count++;
				}
			}
			 
			 }
			 
			//x-1,y
			if((( (x-1) >=0) && ((x-1) < gridWidth)) && ( ( y >= 0) && ( y < gridHeight ) )) {
			 
			Widget w3 = flex.getWidget(x-1, y);
			
			if( (w3 instanceof Button) && (w3.isVisible())){
				if( ((Button) w3).getText().equals("M")){
					count++;
				}
			 }
			 
			}
			
			//x+1,y

			if((( (x+1) >=0) && ((x+1) < gridWidth)) && ( ( y >= 0) && ( y < gridHeight ) )){
			
			Widget w4 = flex.getWidget(x+1 , y);
			
			 if((w4 instanceof Button) && (w4.isVisible())){
				 if( ((Button) w4).getText().equals("M")){
					count++;
				}
			 }
			}
			
			
			//x-1,y-1 
			
			if((( (x-1) >=0) && ((x-1) < gridWidth)) && ( (( y-1) >=0) && ( (y-1) < gridHeight ) )){
				
			
			Widget w5 = flex.getWidget(x-1, y-1);
			if((w5 instanceof Button) && (w5.isVisible())){	 
				 
				if( ((Button) w5).getText().equals("M")){
					count++;
				}
			 }
			}
			
			//x+1,y-1
			if((( (x+1) >=0) && ((x+1) < gridWidth)) && ( (( y-1) >=0) && ( (y-1) < gridHeight ) )){
				
			
			Widget w6 = flex.getWidget(x+1, y-1);
			if((w6 instanceof Button) && (w6.isVisible())){
				if(((Button) w6).getText().equals("M") ){
					count++;
				}
			}
			}
			
			//x+1,y+1
			if((( (x+1) >= 0) && ((x+1) < gridWidth)) && ( (( y+1) >= 0) && ( (y+1) < gridHeight ) )){
			Widget w7 = flex.getWidget(x+1 , y+1);
			
			if((w7 instanceof Button) && (w7.isVisible())){
				
				if( ((Button) w7).getText().equals("M")){
					count++;
				}
			}
			}
			
			//x-1,y+1
			if((( (x-1) >= 0) && ((x-1) < gridWidth)) && ( (( y+1) >=0) && ( (y+1) < gridHeight ) )){
			Widget w8 = flex.getWidget(x-1, y+1);
			
			if((w8 instanceof Button) && (w8.isVisible())){
				
				if( ((Button) w8).getText().equals("M")){
					count++;
				}
			}
			}
			
			
			
			return count;
		
		
		
	}
	
	
	public int randInt(int min, int max) {
		
   		Random rand = new Random();


    	int randomNum = rand.nextInt(max - min) + min;

    	return randomNum;
    	
		}
		

	
	
	
	
}
