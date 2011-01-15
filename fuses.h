#ifdef FUSES
	#if defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__)
	FUSES = {
		.low = FUSE_CKSEL3 & FUSE_SUT0,
		.high = FUSE_SPIEN & FUSE_BOOTSZ0 & FUSE_BOOTSZ1,
		.extended = EFUSE_DEFAULT,
	};
	#else
		#warning No fuse definitions for this chip in fuses.h!
	#endif
#endif	/* FUSES */