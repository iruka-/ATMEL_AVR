[2011-01](2011-01.md) 

## STM32:vcomサンプルのやる気が全然無い件

- たぶん、これがUSARTの受信バッファなのだろう。

<!-- dummy comment line for breaking list -->

	uint8_t  USART_Rx_Buffer [USART_RX_DATA_SIZE]; 
	uint32_t USART_Rx_ptr_in = 0;
	uint32_t USART_Rx_ptr_out = 0;
	uint32_t USART_Rx_length  = 0;
	
	uint8_t  USB_Tx_State = 0;

~
~
~

- とりあえず意味不明コードそのままコピペしてみる。

<!-- dummy comment line for breaking list -->

hw_config.c

	/*******************************************************************************
	* Function Name  : Handle_USBAsynchXfer.
	* Description    : send data to USB.
	* Input          : None.
	* Return         : none.
	*******************************************************************************/
	void Handle_USBAsynchXfer (void)
	{
	 
	 uint16_t USB_Tx_ptr;
	 uint16_t USB_Tx_length;
	 
	 if(USB_Tx_State != 1)
	 {
	   if (USART_Rx_ptr_out == USART_RX_DATA_SIZE)
	   {
	     USART_Rx_ptr_out = 0;
	   }
	   
	   if(USART_Rx_ptr_out == USART_Rx_ptr_in) 
	   {
	     USB_Tx_State = 0; 
	     return;
	   }
	   
	   if(USART_Rx_ptr_out > USART_Rx_ptr_in) /* rollback */
	   { 
	     USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out;
	   }
	   else 
	   {
	     USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
	   }
	   
	   if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
	   {
	     USB_Tx_ptr = USART_Rx_ptr_out;
	     USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
	     
	     USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;	
	     USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;	
	   }
	   else
	   {
	     USB_Tx_ptr = USART_Rx_ptr_out;
	     USB_Tx_length = USART_Rx_length;
	     
	     USART_Rx_ptr_out += USART_Rx_length;
	     USART_Rx_length = 0;
	   }
	   USB_Tx_State = 1; 
	   
	   UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
	   SetEPTxCount(ENDP1, USB_Tx_length);
	   SetEPTxValid(ENDP1); 
	 }  
	 
	}

usb_endp.c

	/*******************************************************************************
	* Function Name  : EP1_IN_Callback
	* Description    :
	* Input          : None.
	* Output         : None.
	* Return         : None.
	*******************************************************************************/
	void EP1_IN_Callback (void)
	{
	 uint16_t USB_Tx_ptr;
	 uint16_t USB_Tx_length;
	 
	 if (USB_Tx_State == 1)
	 {
	   if (USART_Rx_length == 0) 
	   {
	     USB_Tx_State = 0;
	   }
	   else 
	   {
	     if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE){
	       USB_Tx_ptr = USART_Rx_ptr_out;
	       USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
	       
	       USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
	       USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;    
	     }
	     else 
	     {
	       USB_Tx_ptr = USART_Rx_ptr_out;
	       USB_Tx_length = USART_Rx_length;
	       
	       USART_Rx_ptr_out += USART_Rx_length;
	       USART_Rx_length = 0;
	     }
	     UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
	     SetEPTxCount(ENDP1, USB_Tx_length);
	     SetEPTxValid(ENDP1); 
	   }
	 }
	}


	/*******************************************************************************
	* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
	* Description    :
	* Input          : None.
	* Output         : None.
	* Return         : None.
	*******************************************************************************/
	void SOF_Callback(void)
	{
	 static uint32_t FrameCount = 0;
	 
	 if(bDeviceState == CONFIGURED)
	 {
	   if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
	   {
	     /* Reset the frame counter */
	     FrameCount = 0;
	     
	     /* Check the data to be sent through IN pipe */
	     Handle_USBAsynchXfer();
	   }
	 }  
	}


- - - -
- void EP1_IN_Callback (void)は、USBエンドポイント１（device->PC方向）のコールバック。
- void SOF_Callback(void)は、USBの１フレーム時間(1mS)の最初の割り込みのコールバック。

<!-- dummy comment line for breaking list -->


- USARTの受信割り込みハンドラーから呼び出される関数は、次のようになっている。

<!-- dummy comment line for breaking list -->

	/*******************************************************************************
	* Function Name  : UART_To_USB_Send_Data.
	* Description    : send the received data from UART 0 to USB.
	* Input          : None.
	* Return         : none.
	*******************************************************************************/
	void USART_To_USB_Send_Data(void)
	{
	 
	 if (linecoding.datatype == 7)
	 {
	   USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1) & 0x7F;
	 }
	 else if (linecoding.datatype == 8)
	 {
	   USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1);
	 }
	 
	 USART_Rx_ptr_in++;
	 
	 /* To avoid buffer overflow */
	 if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
	 {
	   USART_Rx_ptr_in = 0;
	 }
	}

- - - -
## で、問題点は？？？
	uint8_t  USART_Rx_Buffer [USART_RX_DATA_SIZE]; 
	uint32_t USART_Rx_ptr_in = 0;
	uint32_t USART_Rx_ptr_out = 0;
- 上記FIFO的なバッファに対する操作は、アトミックでなければならないはずだが、セマフォや排他ロックがないこと。
- 上記FIFO的なポインタがバッファサイズを越えた場合０に戻す処理がところどころに入っているけれど、これもアトミック

<!-- dummy comment line for breaking list -->
になっていないため、オーバーランになるタイミングがあり、そのときに割り込んだ場合、どうするんだろう的な突っ込みポイントが存在している。なんとなく。

~
~
- FIFOから取り出して、ＰＣに返送するルーチンが２種類ある。
    - １つは、void Handle_USBAsynchXfer (void)・・・・これはUSBの１フレームの頭でキックされる。（実際には1:5くらいに間引かれている）
    - もう一つは、void EP1_IN_Callback (void)・・・・これはUSBエンドポイント１がＰＣから読まれたらキックされる。

<!-- dummy comment line for breaking list -->

- 上記２つのルーチンは、書き方がそれぞれ微妙に異なっていて、バグッているのかいないのか分からない変なコーディングだ。
    - FIFOキューから取り出す処理は１箇所で書くべきだし、アトミックにすべきだし、一見してバグらないことが分かるようになっていないといけないと思う。

<!-- dummy comment line for breaking list -->


~
- この２つのルーチンの動作はステート（USB_Tx_State）により排他動作になっていて、
    - **USB_Tx_State=0**・・・Handle_USBAsynchXfer()が実行。EP1_IN_Callback()は無効。
        - 一度Handle_USBAsynchXfer()が実行されると、USB_Tx_State=1となる。

<!-- dummy comment line for breaking list -->

    - **USB_Tx_State=1**・・・Handle_USBAsynchXfer()は無効。EP1_IN_Callback()のみ有効。
        - USARTのFIFOが無くなるまでUSB_Tx_State=1が続く。無くなったらUSB_Tx_State=0にリセットされる。

<!-- dummy comment line for breaking list -->

- これだと、一時的にFIFOが途切れたら（RS232C受信データが空になったら）、次にUSB SOFが来るまで何も出来なくなる。
    - つまり、(FRAME_INTERVALという意味不明な間引きカウンタ)5mS程度の時間内に、五月雨式でパラパラUSART割り込みが来たらホストPCが受け取れない空白の時間が発生する。
- こういうこと。

<!-- dummy comment line for breaking list -->
### USB_Tx_State=0 -> 1 -> 0 **<--この期間が魔の時間帯-->** 次のUSB SOFまで

- この文字取りこぼしバグを回避するために、USART_RX_DATA_SIZEを2048などという投げやりなバッファサイズにして逃げている。
- けれども、このバージョンのvcomサンプルは実際に文字化けがぼろぼろ発生するらしい。
- おそらく原因はこれだけではないのだろう。

