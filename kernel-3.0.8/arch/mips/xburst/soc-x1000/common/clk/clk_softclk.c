#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <soc/base.h>

#include "clk.h"
#include "clk_softclk.h"

static DEFINE_SPINLOCK(softclk_lock);

static unsigned int soft_gate = 0;

int i2s_start(void)
{
	__i2s_enable_receive_dma();
	__i2s_enable_record();
	return 0;
}
int i2s_stop(void)
{
	__i2s_disable_record();
	return 0;
}
int dmic_start(void)
{
	__dmic_enable_rdms();
	__dmic_enable();
	return 0;
}
int dmic_stop(void)
{
	__dmic_disable_rdms();
	__dmic_disable();
	return 0;
}

static int softclk_enable(struct clk *clk,int on)
{
	unsigned long flags;
	int bit = CLK_SOFTCLK_BIT(clk->flags);

	if(on)
		soft_gate |= (1 << bit);
	else
		soft_gate &= ~(1 << bit);

	if(on){
		if(soft_gate == 7){ //aec mode
			spin_lock_irqsave(&softclk_lock,flags);
			i2s_start();
			dmic_start();
			spin_unlock_irqrestore(&softclk_lock,flags);
		}else if((bit ==1) && !(soft_gate & 1) ){	//normal amic enable
			spin_lock_irqsave(&softclk_lock,flags);
			i2s_start();
			spin_unlock_irqrestore(&softclk_lock,flags);
		}else if((bit == 2) && !(soft_gate & 1)){	//normal dmic enable
			spin_lock_irqsave(&softclk_lock,flags);
			dmic_start();
			spin_unlock_irqrestore(&softclk_lock,flags);
		}
	}else{
		if(bit == 1){	//amic disable
			spin_lock_irqsave(&softclk_lock,flags);
			i2s_stop();
			soft_gate &= ~1;	//This is just for no write 0 to the aec enable file.
			spin_unlock_irqrestore(&softclk_lock,flags);
		}
		if(bit == 2){	//dmic disable
			spin_lock_irqsave(&softclk_lock,flags);
			dmic_stop();
			spin_unlock_irqrestore(&softclk_lock,flags);
		}
	}
	return 0;
}

static struct clk_ops clk_softclk_ops = {
	.enable = softclk_enable,
};

void __init init_softclk_clk(struct clk *clk)
{
	int id = 0;

	if (clk->flags & CLK_FLG_PARENT) {
		id = CLK_PARENT(clk->flags);
		clk->parent = get_clk_from_id(id);
	}else{
		clk->parent = get_clk_from_id(CLK_ID_EXT1);
	}
	printk("%s,parent is %s\n",__func__,clk->parent->name);
	clk->rate = clk_get_rate(clk->parent);
	clk->ops = &clk_softclk_ops;
}
