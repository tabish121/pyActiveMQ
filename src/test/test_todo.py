class test:
    def xtest_durable_consumer(self):
        conn1 = self.conn
        del self.conn
        session1 = conn1.createSession()
        sub = "mysubscription1"
        # unsubscribing non-existant subscription should fail
        self.assertRaises(UserWarning, session1.unsubscribe, sub)
        topic = self.random_topic(session1)
        consumer = session1.createDurableConsumer(topic, sub, '', False)
        # unsubscribing with an active consumer should fail
        self.assertRaises(UserWarning, session1.unsubscribe, sub)
        del consumer
        # TODO this shouldn't throw
        #session1.unsubscribe(sub)
        consumer = session1.createDurableConsumer(topic, sub, '', False)

        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        conn2 = f.createConnection()
        session2 = conn2.createSession()
        # Using a topic created in another session to create the
        # producer. This shouldn't cause problems.
        producer = session2.createProducer(topic)

        conn1.start()
        conn2.start()

        # TODO send a few messages using the producer
        # TODO receive messages in a transaction
        # TODO rollback transaction
        # TODO close connection
        # TODO reconnect conn1 and setup subscription
        # check that messages are still there

    def test_durable_consumer(self):
        sub = "mysubscription1"

        conn1 = self.conn
        del self.conn
        from pyactivemq import AcknowledgeMode
        session1 = conn1.createSession(AcknowledgeMode.SESSION_TRANSACTED)
        topic = self.random_topic(session1, 'consumer.retroactive=true')
        print topic
        consumer = session1.createDurableConsumer(topic, sub, '', False)

        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        conn2 = f.createConnection()
        session2 = conn2.createSession()
        # Using a topic created in another session to create the
        # producer. This shouldn't cause problems.
        producer = session2.createProducer(topic)
        from pyactivemq import DeliveryMode
        producer.deliveryMode = DeliveryMode.PERSISTENT

        conn1.start()
        conn2.start()

        nmessages = 10
        msg = session2.createTextMessage()

        for i in xrange(nmessages):
            msg.text = 'hello%d' % i
            producer.send(msg)

        # check that the producer sent everything
        for i in xrange(nmessages):
            msg = consumer.receive(1000)
            self.assert_(msg is not None)
            self.assertEqual('hello%d' % i, msg.text)
        session1.rollback()
        conn1.close()
        del conn1

        conn1 = f.createConnection()
        session1 = conn1.createSession(AcknowledgeMode.SESSION_TRANSACTED)
        consumer1 = session1.createConsumer(topic)
        consumer2 = session1.createDurableConsumer(topic, sub, '', False)
        conn1.start()
        msg = consumer1.receive(500)
        self.assert_(msg is None)
        # check that the messages are still available to the consumer
        # with the subscription
        for i in xrange(nmessages):
            msg = consumer2.receive(1000)
            self.assert_(msg is not None)
            self.assertEqual('hello%d' % i, msg.text)
        msg = consumer2.receive(500)
        self.assert_(msg is None)
        session1.commit()

    def xtest_MessageProducer_sends(self):
        # test all send methods of MessageProducer
        # make consumers on 2 topics so that we can check that the
        # topic specified when calling send instead of topic specified
        # when creating producer gets used

        # test that sending to a closed connection fails or something
        pass

