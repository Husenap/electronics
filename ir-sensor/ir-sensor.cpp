#include <bitset>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string_view>
#include <thread>

#include <bcm2835.h>

static constexpr unsigned int SensorPin = RPI_GPIO_P1_12;
static constexpr auto         MaxPulse  = std::chrono::seconds(1);

void signalHandler(int signum) {
  std::cout << "Shutting down" << std::endl;

  bcm2835_close();

  exit(signum);
}

template <typename _Rep, typename _Period>
void waitForPin(uint8_t pin, uint8_t state, const std::chrono::duration<_Rep, _Period>& timeout) {
  const auto t0 = std::chrono::high_resolution_clock::now();
  while (bcm2835_gpio_lev(pin) == state &&
         std::chrono::high_resolution_clock::now() - t0 < timeout) {
    std::this_thread::sleep_for(std::chrono::microseconds(5));
  }
}

template <typename Op>
auto measureTime(Op operation) {
  const auto t0 = std::chrono::high_resolution_clock::now();
  operation();
  return std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::high_resolution_clock::now() - t0);
}

struct Controller {
  struct Pulse {
    int64_t pulse, space;
  };
  // name of the controller
  std::string_view name;
  // number of bits in the scancodes
  uint16_t bits = 32;
  // relative error tolerance 30 = 30%
  uint8_t eps = 30;
  // absolute error tolerance in microseconds
  int64_t aeps = 100;

  // header  - the initial pulse and space
  Pulse header;
  // one - the pulse and space representing a one
  Pulse one;
  // zero - the pulse and space representing a zero
  Pulse zero;

  // repeat - the pulse and space for repeating a signal
  Pulse repeat;

  // gap - a long pulse after
  int64_t gap;

  bool isWithinRange(int64_t input, int64_t target) const {
    const int64_t error = std::max(target * eps / 100, aeps);
    return input >= target - error && input <= target + error;
  }
  bool match(Pulse input, Pulse target) const {
    return isWithinRange(input.pulse, target.pulse) && isWithinRange(input.space, target.space);
  }
  Pulse readPulse() const {
    return {measureTime([]() { waitForPin(SensorPin, LOW, MaxPulse); }).count(),
            measureTime([]() { waitForPin(SensorPin, HIGH, MaxPulse); }).count()};
  }
};

static constexpr Controller Andersson{
    .name = "Andersson",
    .bits = 32,
    .eps  = 30,
    .aeps = 100,

    .header = {9500, 4400},
    .one    = {674, 1500},
    .zero   = {674, 440},

    .repeat = {9500, 2126},

    .gap = 108500,
};

int main() {
  std::signal(SIGINT, signalHandler);

  if (!bcm2835_init()) {
    std::cerr << "Failed to initialize bcm2835 library!" << std::endl;
    return 1;
  }

  bcm2835_gpio_fsel(SensorPin, BCM2835_GPIO_FSEL_INPT);

  constexpr Controller controller{Andersson};
  std::cout << "Loaded controller: " << controller.name << '\n';
  while (true) {
    const auto pulse = controller.readPulse();
    if (controller.match(pulse, controller.header)) {
      std::cout << "Recieved: header" << '\n';
    } else if (controller.match(pulse, controller.repeat)) {
      std::cout << "Recieved: repeat" << '\n';
    }
    // std::cout << pulseTime.count() << ' ' << spaceTime.count() << '\n';
  }
}

/*
===================================
https://www.lirc.org/html/lircd.conf.html
https://www.digikey.se/en/maker/blogs/2021/how-to-send-and-receive-ir-signals-with-a-raspberry-pi
===================================

andersson
01011111 11011111 10011111 00011111
01101111 11101111 10101111 00101111
01001111 11001111 10001111 00001111
01010111 11010111 10010111 00010111
01100111 11100111 10100111 00100111
01110111 11110111 10110111 00110111


00000000 11111111 11100000 00011111

107793 108477

1 1300039
1 1300003
1 1300025
1 20662
9599 4353  header
697 444    zero
673 460    zero
646 474    zero
653 446    zero
689 449    zero
653 449    zero
677 410    zero
701 448    zero
678 1552   one
618 1538   one
725 1493   one
705 1530   one
681 1489   one
728 1527   one
695 1513   one
739 1480   one
700 1585   one
633 1570   one
689 1491   one
704 403    zero
712 450    zero
664 429    zero
637 454    zero
664 430    zero
695 420    zero
697 413    zero
702 445    zero
686 2008   one
213 1555   one
685 1564   one
927 1266   one
654 1603   one
629 39804
9508 2160
687 96122
9547 2142
656 96189
9470 2121
719 96163
9509 2116
719 96141
9547 2074
717 96127
9603 2094
643 96229
9463 2160
635 96213
9525 2097
696 96196
9461 2170
642 96161
9556 2116
702 96143
9519 2145
568 1300048
0 579507
179 1300079



1 1300060
1 1239019
9642 4435
597 496
696 461
615 502
581 497
615 488
722 434
633 450
682 473
624 1638
639 1559
594 1613
703 1585
559 1662
624 1592
632 1566
600 1605
658 1600
674 1561
713 1498
643 540
572 469
702 444
638 489
629 483
623 542
662 408
677 476
641 1582
639 1598
641 1602
638 1471
743 1509
703 39723
9689 2185
654 95673
9572 2179
615 96109
9527 2263





SAMSUNG
0 1300048
1 1300063
1 1300040
0 1300025
1 1300041
1 1127316
4591 4363
620 1566
609 1589
607 1593
632 482
562 436
627 461
602 462
622 481
611 1531
647 1566
650 1578
647 635
364 496
589 447
631 422
635 460
833 221
636 1578
652 379
639 499
628 431
645 437
638 420
639 443
631 1591
639 408
637 1532
686 1534
625 1525
680 1562
649 1515
629 1552
577 46705
4564 4374
669 1537
621 1586
567 1579
651 464
578 511
631 375
963 100
580 427
681 1550
655 1518
654 1520
656 466
637 415
646 438
572 467
619 462
666 385
707 1504
615 471
618 429
637 459
623 427
647 417
660 457
586 1562
663 401
626 1599
630 1551
780 1447
598 1592
608 1559
621 1553
689 46646
4544 4429
627 1555
693 1478
634 1573
624 442
670 401
622 425
646 646
420 407
678 1527
650 1566
615 1571
632 464
643 415
607 439
633 462
614 414
640 464
676 1490
676 442
592 473
592 479
637 468
571 475
911 146
653 1536
629 424
652 1598
611 1532
682 1538
671 1565
574 1609
588 1595
609 46642
4627 4370
651 1556
749 1412
656 1547
652 445
571 447
607 437
662 420
582 443
663 1548
606 1615
607 1577
597 653
483 405
676 403
609 495
617 465
577 467
607 1608
609 409
681 380
650 438
677 372
655 475
880 165
650 1759
383 482
782 1415
631 1622
479 1604
623 1559
621 1540
631 1544
640 46704
4584 4358
635 1553
887 1279
764 1417
675 449
627 430
603 451
610 497
610 424
692 1538
650 1536
655 1505
634 487
597 448
636 419
662 461
719 316
598 527
543 1579
611 488
602 514
540 451
669 422
650 419
629 451
645 1556
663 411
636 1554
646 1498
652 1530
635 1561
670 1562
606 1558
651 46610
4586 4395
642 1590
634 1544
630 1510
680 412
656 412
669 434
654 376
655 483
569 1602
861 1286
635 1553
645 424
687 425
613 430
660 427
624 467
653 463
579 1606
592 418
657 398
665 460
594 435
775 307
687 377
613 1610
576 463
643 1542
675 1536
667 1519
808 1384
612 1547
727 1469
637 46679
4566 4364
657 1592
582 1564
628 1583
662 406
615 444
659 410
689 362
640 442
640 1582
631 1775
521 1420
663 452
592 487
788 284
652 379
650 470
633 444
615 1540
665 428
672 382
714 380
679 466
571 411
684 417
621 1526
649 458
642 1577
609 1571
644 1559
692 1501
589 1596
609 1610
605 46656
4573 4407
599 1575
621 1590
585 1575
608 439
668 468
614 419
680 403
696 406
563 1569
603 1580
603 1589
651 399
641 452
587 469
631 440
648 401
639 493
585 1566
667 420
624 417
622 438
656 425
693 350
672 431
648 1529
639 421
681 1523
973 1236
637 1531
634 1595
583 1621
644 1554
604 46670
4563 4395
607 1609
621 1571
642 1558
595 471
718 373
621 566
512 409
728 337
701 1562
625 1526
660 1535
634 474
621 406
648 451
678 408
663 406
615 435
635 1572
599 451
632 477
619 432
625 466
567 501
636 412
674 1542
647 456
600 1571
637 1515
739 1497
602 1571
601 1563
662 1548
896 46367
4597 4396
575 1615
753 1420
712 1460
576 433
679 464
754 319
584 436
706 406
622 1546
694 1511
680 1531
617 472
587 434
648 450
761 279
677 638
430 433
611 1564
680 409
705 426
618 443
592 504
644 408
635 432
629 1524
653 476
612 1543
670 1538
601 1599
618 1537
594 1617
605 1571
645 1300008

*/
